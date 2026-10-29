#ifndef IMOL_LICENSE_H
#define IMOL_LICENSE_H

#include "core/common.h"

struct LicenseData;
class License : public QObject
{
    Q_OBJECT

public:
    enum ErrorType {
        Success,
        NoLic,
        ParseFailed,
        ParseFailedExtra,
        Expired,
        NoFile,
        FileInvalid,
        FileUnsafe
    };
    Q_ENUM(ErrorType)

    struct Error {
        ErrorType type;
        bool legal;
        QString msg;
    };

public:
    explicit License(QObject *parent = nullptr);
    ~License();

public:
    static QString md5(const QByteArray &bytes, const QByteArray &disturb);

public:
    imol::ModuleObject * mobj() const;
    void setLicPath(const QString &path);
    bool involute();
    Error check();

protected:
    LicenseData *d;
};

#endif // IMOL_LICENSE_H
