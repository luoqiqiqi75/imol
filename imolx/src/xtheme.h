#ifndef XTHEME_H
#define XTHEME_H

#include "xhelper.h"

class XTheme : public ix::Module
{
    Q_OBJECT

public:
    explicit XTheme(QObject *parent = nullptr);

    QString name() const override;

public:
    static QPixmap recolorPixmap(const QPixmap &pixmap, const QColor &from, const QColor &to);

public:
    template<typename T>
    T asset(const QString &item, const QString &key, const std::function<T(const QString &)> &defaut_func) const
    {
        QVariant v = mobj()->c(item)->r(key)->get();
        if (v.isNull()) return defaut_func(key);
        if (v.type() != QVariant::String) return v.value<T>();
        QString str = v.toString();
        if (key == str) return defaut_func(key);
        return asset(item, str, defaut_func);
    }

    QString constructQss(const QString &styled_qss) const;

private:
    void setup() override;
};

#endif // XTHEME_H
