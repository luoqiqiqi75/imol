#include "license.h"

#include "bwaf/bfactory.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QElapsedTimer>
#include <QProcess>

IMOL_VER_REG("imol.license", 1)

struct LicenseData
{
    imol::ModuleObject *mobj;
    QString internal_path;
    QString lic_path;

    explicit LicenseData()
    {
        internal_path = "internal_license.json";

        QFileInfo fi(QCoreApplication::applicationFilePath());
        lic_path = QString("%1/%2.lic").arg(fi.path(), fi.baseName());
    }
};

License::License(QObject *parent) : QObject(parent), d(new LicenseData)
{
    d->mobj = m().regist(this, "imol.lic");
}

License::~License()
{
    delete d;
}

QString License::md5(const QByteArray &bytes, const QByteArray &disturb)
{
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(disturb);
    md5_hash.addData(QString("[infinite modules on line]%1").arg(73).toUtf8());
    md5_hash.addData(bytes);
    return QString(md5_hash.result().toHex());
}

QString file_md5(const QString &path)
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

imol::ModuleObject * License::mobj() const { return d->mobj; }
void License::setLicPath(const QString &path) { d->lic_path = path; }

bool License::involute()
{
    if (!QFile::exists(d->internal_path)) return false;

    // json
    imol::ModuleObject lic_mobj("lic");
    lic_mobj.importFromJson(this, m().readFromJson(d->internal_path));

    // file
    foreach (imol::ModuleObject *file_mobj, lic_mobj.c("file")->cmobjs()) {
        file_mobj->set(this, "md5", file_md5(file_mobj->getString()));
    }

    // date
    lic_mobj.set(this, "_date", QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));

    // signature
    lic_mobj.set(this, "sig.api", IMOL_API);
    lic_mobj.set(this, "sig.author", "rokae"); // temp
    lic_mobj.set(this, "sig.expired", QDate(2032, 12, 1)); // temp
    lic_mobj.set(this, "sig.json", file_md5(d->internal_path));
    lic_mobj.set(this, "sig", md5(lic_mobj.exportToBin(false), "who cares your work?"));

    m().writeToBin(d->lic_path, lic_mobj.exportToBin(false), 2);
    return true;
}

QString modulesCheck(imol::ModuleObject *modules_mobj, const QHash<QString, bool> &files)
{
    foreach (imol::ModuleObject *module_mobj, modules_mobj->cmobjs()) {
        if (!files.contains(module_mobj->c("path")->getString())) return module_mobj->c("path")->getString();
        QString sub_check = modulesCheck(module_mobj->c("submodule"), files);
        if (!sub_check.isEmpty()) return sub_check;
    }
    return "";
}

License::Error License::check()
{
    // existance
    if (!QFile::exists(d->lic_path)) return {License::NoLic, false, d->lic_path};
    d->mobj->importFromBin(this, m().readFromBin(d->lic_path), true, true, true);

    // signature
    imol::ModuleObject *sig_mobj = d->mobj->c("sig");
    if (sig_mobj->isEmptyMobj()) return {License::ParseFailed, false, ""};

    QDate expired = sig_mobj->c("expired")->get().toDate();
    DLOG << "<LICENSE> api: " << sig_mobj->c("api")->getInt()
         << ", auther: " << sig_mobj->c("author")->getString()
         << ", expired: " << expired;
    QString sig_md5 = sig_mobj->getString();
    sig_mobj->set(this, QVariant());
    QString real_sig_md5 = md5(d->mobj->exportToBin(false), "who cares your work?");
    d->mobj->remove(this, "sig");

    if (sig_md5 != real_sig_md5) return {License::ParseFailedExtra, false, ""};
    if (QDate::currentDate() > expired) return {License::Expired, false, expired.toString()};

    // files
    QHash<QString, bool> files;
    files.reserve(d->mobj->c("file")->size());
    foreach (imol::ModuleObject *file_mobj, d->mobj->c("file")->cmobjs()) {
        QString md5 = file_md5(file_mobj->getString());
        if (md5.isEmpty()) return {License::NoFile, false, file_mobj->getString()};
        if (md5 != file_mobj->c("md5")->getString()) return {License::FileInvalid, false, file_mobj->getString()};
        files.insert(file_mobj->getString(), true);
    }
    d->mobj->remove(this, "file");
    QString modules_check = modulesCheck(d->mobj->c("module"), files);
    if (!modules_check.isEmpty()) return {License::FileUnsafe, false, modules_check};

    return {License::Success, true, ""};
}
