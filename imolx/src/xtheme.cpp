#include "xtheme.h"

#include "core/commandmanager.h"
#include "bwaf/centerwidget.h"
#include "bwaf/bfactory.h"

#include <QApplication>
#include <QDir>

#include "xsupport.h"

#define NAME "xtheme"

IMOL_VER_REG(mName(MODULE_NAME, NAME), 6)

BFACTORY_REGIST_MODULE(mName(MODULE_NAME, NAME), XTheme)

XTheme::XTheme(QObject *parent) : ix::Module(parent)
{
    bindMobjJson(NAME, ix::templatePath(NAME));
    mobj()->copyFrom(this, m("imol.cfg.xtheme"));

    double zoom = m("imol.cfg.env.qt_scale_factor")->getDouble(1);
    mobj()->set(this, "zoom", zoom);

    // font asset
    qApp->setFont(ix::xtheme::font("default"));

    // color asset
//    auto color_node = mobj()->c("color");

    // pixmap asset
    auto pixmap_node = mobj()->c("pixmap");

    auto sys_foreground_color = ix::xtheme::color("sys.foreground");

    auto recolor = [=] (const QString &pixmap_name) {
        QPixmap pixmap = QPixmap(ix::imagePath(zoom > 1.01f ? pixmap_name + "@2x" : pixmap_name));
        if (pixmap.isNull() && zoom > 1.01f) pixmap = QPixmap(ix::imagePath(pixmap_name));
        return sys_foreground_color == QColor(0, 0, 0) ? pixmap : ix::xtheme::pixmap(pixmap, sys_foreground_color);
    };

    auto split = [=] (const QString &ori_name, const QStringList &split_names) {
        QPixmap p = recolor(ori_name);
        int h = p.height();
        for (int i = 0; i < split_names.size(); i++) {
            pixmap_node->set(this, mName(ori_name, split_names.at(i)), p.copy(h * i, 0, h, h));
        }
    };

    QHash<QString, QStringList> split_hash;

    QStringList udlr = QStringList() << "up" << "down" << "left" << "right";
    split_hash.insert("arrow", udlr);
    split_hash.insert("triangle", udlr);
    split_hash.insert("direction", udlr);
    split_hash.insert("doubledirection", udlr);

    split_hash.insert("window", QStringList() << "min" << "max" << "normal" << "close");
    split_hash.insert("box", QStringList() << "radio.off" << "radio.on" << "check.off" << "check.on");

    split_hash.insert("calculate", QStringList() << "plus" << "minus" << "times" << "divide");
    split_hash.insert("edit", QStringList() << "undo" << "redo" << "cut" << "copy" << "paste" << "delete");

    foreach (const QFileInfo &fi, QDir(":/imolx/images/").entryInfoList()) {
        QString name = fi.baseName();
        if (name.contains("@")) continue;
        if (split_hash.contains(name)) {
            split(name, split_hash.value(name));
        } else {
            pixmap_node->set(this, name, recolor(name));
        }
    }
}

QString XTheme::name() const { return NAME; }

QPixmap XTheme::recolorPixmap(const QPixmap &pixmap, const QColor &from, const QColor &to)
{
    if (!to.isValid()) {
        ELOG << "<xtheme> recolor pixmap with invalid color : " << from << " -> " << to;
        return pixmap;
    }

    int fh = from.hsvHue();
    QImage img = pixmap.toImage();
#ifdef XSUPPORT_QT4
    QImage img_a = img.alphaChannel();
    QRgb trgb = to.rgb();
#else
    QColor tc = to;
#endif
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            QRgb rgba = img.pixel(x, y);
            int a = qAlpha(rgba);
            if (a == 0) continue;
            if (!from.isValid() || QColor::fromRgba(rgba).hsvHue() == fh) {
#ifdef XSUPPORT_QT4
                img.setPixel(x, y, trgb);
#else
                tc.setAlpha(a);
                img.setPixelColor(x, y, tc);
#endif
            }
        }
    }
#ifdef XSUPPORT_QT4
    img.setAlphaChannel(img_a);
#endif
    return QPixmap::fromImage(img);
}

QString XTheme::constructQss(const QString &styled_qss) const
{
    static QRegExp rx("\\\"(\\w+)@(.+)\\\"");
    rx.setMinimal(true);
    QString qss;
    qss.reserve(styled_qss.length());
    int offset = 0;
    while (offset < styled_qss.length()) {
        int index = rx.indexIn(styled_qss, offset);
        qss.append(styled_qss.midRef(offset, index < 0 ? -1 : index - offset));
        if (index < 0) break;

        QString type = rx.cap(1), key = rx.cap(2);
        if (type == "color") {
            qss.append(ix::xtheme::color(key).name());
        } else if (type == "font") {
            qss.append(ix::xtheme::font(key).toString());
        } else {
            WLOG << "<xtheme> unknown qss: " << rx.cap();
        }
        offset = index + rx.matchedLength();
    }
    return qss;
}

void exportPixmapNode(const QString &path, ix::Node *pn)
{
    if (pn->hasCmobj()) {
        QDir::current().mkpath(path);
        foreach (ix::Node *n, pn->cmobjs()) {
            exportPixmapNode(QString("%1/%2").arg(path, n->name()), n);
        }
    } else {
        QPixmap p = pn->get().value<QPixmap>();
        if (!p.isNull()) p.save(path + ".png", "PNG");
    }
}

void exportFont(const QString &path, ix::Node *fn)
{
    Q_UNUSED(path)
    Q_UNUSED(fn)
    // todo
}

void XTheme::setup()
{
    QString path = QString("./asset/%1").arg(MODULE_NAME);
    exportPixmapNode(path, mobj()->c("pixmap"));
    exportFont(path, mobj()->c("font"));
}
