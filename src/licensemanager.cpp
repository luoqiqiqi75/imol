#include "licensemanager.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QProcess>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>

#include "logmanager.h"

LicenseManager::LicenseManager(QObject *parent) : QObject(parent),
    m_path(""),
    m_is_legal(false)
{
}

LicenseManager::~LicenseManager()
{
}

LicenseManager & LicenseManager::instance()
{
    static LicenseManager manager;
    return manager;
}

QString LicenseManager::md5(const QByteArray &bytes)
{
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(QString("%1%2%3%4%5%6%7").arg('l').arg('u').arg('o').arg('s').arg('h').arg('i').arg(2014).toUtf8());
    md5_hash.addData(bytes);
    md5_hash.addData(QByteArray('a', (char)211));
    md5_hash.addData(QByteArray('i', (char)985));
    return QString(md5_hash.result().toHex());
}

QString LicenseManager::md5(const QByteArray &bytes, const QByteArray &disturb)
{
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(disturb);
    md5_hash.addData(QString("[infinite modules on line]%1").arg(73).toUtf8());
    md5_hash.addData(bytes);
    return QString(md5_hash.result().toHex());
}

QString LicenseManager::fileMD5(const QString &path)
{
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(QString("%1%2%3%4%5%6").arg('r').arg('o').arg('k').arg('a').arg('e').arg(2014).toUtf8());

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return "";
    md5_hash.addData(&file);
    file.close();

    md5_hash.addData(QByteArray('o', 7));
    return QString(md5_hash.result().toHex());
}

QString LicenseManager::defaultInternalLicFileName()
{
    return "internal_license.json";
}

QString LicenseManager::defaultLicFileName()
{
    QFileInfo fi(QCoreApplication::applicationFilePath());
    return QString("%1/%2.lic").arg(fi.path()).arg(fi.baseName());
}

bool LicenseManager::loadFromFile(const QString &path)
{
    m_path = path;
    m_content = QJsonObject();

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;

    QByteArray bytes = f.readAll();
    f.close();

    QList<QByteArray> bytes_array = bytes.split('+');
    if (bytes_array.size() != 2) return false;

    QByteArray content_bytes = bytes_array.at(0);
    QByteArray check_bytes = bytes_array.at(1);

    if (md5(content_bytes) != QString::fromUtf8(check_bytes)) return false;

    QJsonParseError json_error;
    QJsonDocument json_doc = QJsonDocument::fromJson(QByteArray::fromBase64(content_bytes), &json_error);
    m_content = json_doc.object();

    return json_error.error == QJsonParseError::NoError;
}

bool LicenseManager::buildToFile(const QJsonObject &content, const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;

    QJsonDocument json_doc(content);
    QByteArray bytes = json_doc.toJson(QJsonDocument::Compact).toBase64(QByteArray::OmitTrailingEquals);
    f.write(bytes);
    f.write("+");
    f.write(md5(bytes).toUtf8());
    f.close();

    return true;
}

bool LicenseManager::buildFromFile(const QString &src_path, const QString &tar_path)
{
    QFile f(src_path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QByteArray json_bytes = f.readAll();
    f.close();

    QJsonObject json = QJsonDocument::fromJson(json_bytes).object();
    QJsonObject file_json = json.value("file").toObject();
    QStringList file_paths = file_json.keys();
//    QStringList file_paths = file_json.count() > 0 ? file_json.keys() : QDir().entryList(QStringList() << "*.dll", QDir::Files | QDir::NoDotAndDotDot);
    foreach (QString file_path, file_paths) {
        file_json.insert(file_path, fileMD5(file_path));
    }
    json.insert("file", file_json);

    json.insert("#date", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));

    return buildToFile(json, tar_path);
}

bool LicenseManager::addItem(const QString &key, const QJsonValue &value)
{
    if (m_content.contains(key)) return false;
    m_content.insert(key, value);
    return true;
}

QJsonValue LicenseManager::item(const QString &key)
{
    return m_is_legal ? m_content.value(key) : QJsonValue();
}

LicenseError LicenseManager::checkProgrammer()
{
    if (!QFile("internal_license.json").exists()) return LicenseError(LicenseError::UNKNOWN_ERROR);

//    QProcess check_cmd(this);
//    check_cmd.start("curl ip.sb");
//    check_cmd.waitForFinished(10000);
//    QString ip = QString::fromLocal8Bit(check_cmd.readAllStandardOutput()).trimmed();
//    return LicenseError(ip == "114.242.25.17" ? LicenseError::NO_LICENSE_ERROR : LicenseError::UNKNOWN_IP_ERROR);

    QUrl url("ftp://10.0.4.30/hallo");
    url.setUserName("qilu");
    url.setPassword("426#tSc%J#jhNPj");
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    QNetworkReply *rpl = nam->get(QNetworkRequest(url));
    int cnt = 0;
    while (rpl->isOpen() && !rpl->isFinished() && cnt++ < 300000) QCoreApplication::processEvents();
    bool success = rpl->error() == QNetworkReply::NoError && QString::fromUtf8(rpl->readLine(10)) == "genau";
    rpl->deleteLater();
    nam->deleteLater();
    return success ? LicenseError::NO_LICENSE_ERROR : LicenseError::UNKNOWN_IP_ERROR;
}

LicenseError LicenseManager::checkLegitimacy()
{
    m_is_legal = false;
    if (!m_content.contains("file")) return LicenseError(LicenseError::LICENSE_NOT_EXIST);

    QJsonObject files_json = m_content.value("file").toObject();
    foreach (QString path, files_json.keys()) {
        QString md5 = LicenseManager::fileMD5(path);
        if (md5.isEmpty()) return LicenseError(LicenseError::FILE_NOT_EXIST, path);
        if (md5 != files_json.value(path).toString("")) return LicenseError(LicenseError::FILE_MD5_MISMATCH, path);
    }

    QJsonArray modules_json = m_content.value("module").toArray();
    foreach (QJsonValue module_value, modules_json) {
        QJsonObject module_json = module_value.toObject();
        QString module_path = module_json.value("path").toString();
        if (!files_json.contains(module_path)) return LicenseError(LicenseError::MODULE_NOT_CHECKED, module_path);
    }

    m_is_legal = true;
    return LicenseError();
}

bool LicenseManager::isLegal()
{
    return m_is_legal;
}

QStringList LicenseManager::modulePathList()
{
    if (!m_is_legal) return QStringList();

    QStringList module_paths;
    QJsonArray modules_json = m_content.value("module").toArray();
    foreach (QJsonValue module_value, modules_json) {
        module_paths.append(module_value.toObject().value("path").toString());
    }
    return module_paths;
}

QStringList LicenseManager::appPathList(const QString &module_path)
{
    if (!m_is_legal) return QStringList();

    QJsonArray modules_json = m_content.value("module").toArray();
    foreach (QJsonValue module_value, modules_json) {
        QJsonObject module_json = module_value.toObject();
        if (module_path == module_json.value("path").toString()) {
            QStringList app_paths;
            QJsonArray apps_array = module_json.value("app").toArray();
            foreach (QJsonValue app_value, apps_array) {
                app_paths.append(app_value.toString());
            }
            return app_paths;
        }
    }
    return QStringList();
}

LicenseManager & license()
{
    return LicenseManager::instance();
}
