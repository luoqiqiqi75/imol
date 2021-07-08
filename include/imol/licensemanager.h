#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H

#include "core_global.h"

#include <QObject>
#include <QJsonObject>
#include <QHash>

class CORESHARED_EXPORT LicenseError
{
public:
    enum LicenseErrorType {
        NO_LICENSE_ERROR,
        LICENSE_NOT_EXIST,
        LICENSE_PARSE_ERROR,
        FILE_NOT_EXIST,
        FILE_MD5_MISMATCH,
        MODULE_NOT_CHECKED,
        APP_NOT_CHECKED,
        UNKNOWN_IP_ERROR,
        UNKNOWN_ERROR
    };

    LicenseError(const QJsonValue &arg = QJsonValue()) : m_type(NO_LICENSE_ERROR), m_arg(arg) {}
    LicenseError(LicenseErrorType type, const QJsonValue &arg = QJsonValue()) : m_type(type), m_arg(arg) {}

    LicenseErrorType type() {return m_type;}
    void setType(LicenseErrorType type) {m_type = type;}
    bool hasNoError() {return m_type == NO_LICENSE_ERROR;}

    QJsonValue arg() {return m_arg;}
    void setArg(const QJsonValue &arg) {m_arg = arg;}

private:
    LicenseErrorType m_type;
    QJsonValue m_arg;
};

class QFile;
class CORESHARED_EXPORT LicenseManager : public QObject
{
    Q_OBJECT

public:
    explicit LicenseManager(QObject *parent = nullptr);
    ~LicenseManager();
    static LicenseManager & instance();

    static QString md5(const QByteArray &bytes);
    static QString md5(const QByteArray &bytes, const QByteArray &disturb);
    static QString fileMD5(const QString &path);

    QString defaultInternalLicFileName();
    QString defaultLicFileName();

    bool loadFromFile(const QString &path);
    bool buildToFile(const QJsonObject &content, const QString &path);
    bool buildFromFile(const QString &src_path, const QString &tar_path);

    bool addItem(const QString &key, const QJsonValue &value);
    QJsonValue item(const QString &key);

    LicenseError checkProgrammer();
    LicenseError checkLegitimacy();
    bool isLegal();

    QStringList modulePathList();
    QStringList appPathList(const QString &module_path);

private:
    QString m_path;
    QJsonObject m_content;
    bool m_is_legal;
};
//output methods
CORESHARED_EXPORT LicenseManager & license();

#endif // LICENSEMANAGER_H
