#include "imol.h"

#include <iostream>

#include "core/common.h"

#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QObject>
#include <QSplashScreen>
#include <QSettings>
#include <QDir>
#include <QFontDatabase>
#include <QIcon>
#include <QTimer>
#include <QJsonDocument>

#ifdef WIN64
#include "util/StackWalker/BaseException.h"
#endif

#include "server.h"
#include "config.h"
#include "license.h"
#include "core/common.h"

IMOL_VER_REG("imol.main", 13)

#define LANGUAGE_EN "en"
#define LANGUAGE_CN "cn"

void rescue(imol::ModuleObject *cfg_mobj)
{
    // activate
    bool activate = cfg_mobj->c("activate")->getBool();
    if (activate) {
        QString path = cfg_mobj->c("activate_path")->getString();
        QString file_name = cfg_mobj->c("activate_file_name")->getString();
        QString current_path = QDir::current().absolutePath();
        ILOG << "<rescue> enable activate: " << path << "/" << file_name << ".lic";
        QDir::setCurrent(path);
        License lic;
        lic.setLicPath(QString("%1/%2.lic").arg(path, file_name));
        bool success = lic.involute();
        ILOG << "<rescue> activate " << (success ? "succeed" : "failed");
        QDir::setCurrent(current_path);
    }
}

int main(int argc, char *argv[])
{
    SLOG << "[imol version: " << imol::releaseVersionString("imol") << "]";

    ILOG << "------------------------";
    ILOG << "start with parameters:";
    for (int i = 0; i < argc; ++i) ILOG << argv[i];
    ILOG << "------------------------";

#ifdef WIN64
    SET_DEFAULT_HANDLER();
#endif

    // configuation
    Config cfg;
    cfg.loadBeforeApplication();

    // rescue_mode
    if (cfg.mobj("admin.rescue_mode")->getBool()) {
        ILOG << "<rescue> enter rescue mode";
        QCoreApplication ca(argc, argv);
        rescue(cfg.mobj("admin"));
        if (cfg.mobj("admin.console")->getBool()) {
            // todo setup console module
            return ca.exec();
        } else {
            return 0;
        }
    }

    // scaling
    double scale_factor = cfg.mobj("env.qt_scale_factor")->getDouble();
    if (scale_factor > 0.999) qputenv("QT_SCALE_FACTOR", cfg.mobj("env.qt_scale_factor")->getString().toStdString().c_str());
    if (scale_factor > 1.001) {
        QFile conf("qt.conf");
        if (conf.open(QIODevice::WriteOnly)) {
            conf.write("[Platforms]\nWindowsArguments=fontengine=freetype");
            conf.close();
        }
    } else {
        QFile conf("qt.conf");
        if (conf.exists()) {
            conf.setPermissions(QFile::WriteUser);
            conf.remove();
        }
    }

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, cfg.mobj("attr.share_opengl_contexts")->getBool(true));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, cfg.mobj("attr.enable_high_dpi_scaling")->getBool(false));
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, cfg.mobj("attr.use_high_dpi_pixmaps")->getBool(false));

    // application
    QApplication a(argc, argv);
    QApplication::setApplicationName("imol");

    // load assets
    const QDir font_dir(":/imol/asset/font/");
    foreach (const QFileInfo &fi, font_dir.entryInfoList({"*.ttf"}, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
        QFontDatabase::addApplicationFont(fi.filePath());
    }
    const QString lang_path(":/imol/language/imol_%1.json");
    translator().installJsonTranslator(QApplication::applicationName(), lang_path.arg(cfg.mobj("common.language")->getString(LANGUAGE_CN)));

    // license control
    License lic;
    if (lic.involute()) ILOG << "<LICENSE> created";

    License::Error lic_err = lic.check();
    switch (lic_err.type) {
    case License::Success: break;
    case License::NoLic: case License::NoFile:
        QMessageBox::critical(nullptr, QObject::tr("File error"),
                              QObject::tr("Cannot find %1!").arg(lic_err.msg));
        break;
    case License::ParseFailed: case License::ParseFailedExtra:
        QMessageBox::critical(nullptr, QObject::tr("License error"),
                              QObject::tr("No valid license file!"));
        break;
    case License::Expired:
        QMessageBox::critical(nullptr, QObject::tr("License error"),
                              QObject::tr("Authorization expired!"));
        break;
    case License::FileInvalid: case License::FileUnsafe:
        QMessageBox::critical(nullptr, QObject::tr("File error"),
                              QObject::tr("Error occurs while loading %1!").arg(lic_err.msg));
        break;
    default: break;
    }

    if (!lic_err.legal) { // exit when illegal
        ELOG << "<LICENSE> illegal: " << lic_err.type << " " << lic_err.msg;
        return 0;
    }

    // prepare environment
    if (lic.mobj()->hasCmobj("env")) {
#if defined(Q_OS_WIN)
        QString env_key = "PATH";
        QString env_separator = ";";
#elif defined(Q_OS_LINUX)
        QString env_key = "LD_LIBRARY_PATH";
        QString env_separator = ":";
#endif
        QString full_env_path = qgetenv(env_key.toStdString().c_str());
        QString app_dir = QCoreApplication::applicationDirPath();
        foreach (imol::ModuleObject *env_mobj, lic.mobj()->c("env")->cmobjs()) {
            QString env_path = env_mobj->getString();
            if (env_path.startsWith(".")) env_path.replace(0, 1, app_dir);
            env_path.replace("/", QDir::separator());
            full_env_path.append(env_separator + env_path);
        }
        qputenv(env_key.toStdString().c_str(), full_env_path.toLocal8Bit());
    }

    // splash screen
    bool splash_enable = lic.mobj()->r("splash.enable")->getBool(true);
    QSplashScreen *splash_screen = nullptr;
    if (splash_enable) {
        QIcon splash_icon;
        QSize splash_size;
        QString splash_path = lic.mobj()->r("splash.pixmap")->getString();
        if (splash_path.isEmpty()) {
            QPixmap p(DEFAULT_SPLASH_PIXMAP);
            splash_icon.addPixmap(p);
            splash_size = p.size();
            if (scale_factor > 1.01) splash_icon.addPixmap(QPixmap(DEFAULT_SPLASH_PIXMAP_HIGH_DPI));
        } else {
            QPixmap p(splash_path);
            splash_icon.addPixmap(p);
            splash_size = p.size();
        }
        if (lic.mobj()->hasRmobj("splash.size")) {
            splash_size.setWidth(lic.mobj()->r("splash.size.w")->getInt());
            splash_size.setHeight(lic.mobj()->r("splash.size.h")->getInt());
        }
        splash_screen = new QSplashScreen(splash_icon.pixmap(splash_size));

        //start imol with splash
        splash_screen->setFont(QFont(lic.mobj()->r("splash.font_family")->getString("Microsoft YaHei UI"),
                                    lic.mobj()->r("splash.font_size")->getInt(-1),
                                    lic.mobj()->r("splash.font_weight")->getInt(-1),
                                    lic.mobj()->r("splash.font_italic")->getBool(false)));
        if (!splash_screen->pixmap().isNull()) splash_screen->show();
    }

    // start server
    Server server;
    if (lic.mobj()->r("server.enable")->getBool(true)) server.start(lic.mobj()->r("server.port")->getInt(5059));
    a.processEvents();

    // start imol
    Imol imol(splash_screen);
    if (splash_screen) {
        splash_screen->finish(imol.wgt());
        splash_screen->deleteLater();
    }
    if (lic.mobj()->c("show")->getBool(true)) imol.wgt()->show();

    if (a.arguments().contains("--smoke-test")) {
        QTimer::singleShot(500, &a, [&] {
            auto* base = imol.findSubmodule("imolx");
            QJsonObject result;
            result["program"] = imol.moduleState() == bwaf::BModule::STATE_IDLE;
            result["imolx"] = base && base->moduleState() == bwaf::BModule::STATE_IDLE;
            bool passed = result["program"].toBool() && result["imolx"].toBool();
            for (const QString& name : {QString("xtheme"), QString("xservice"), QString("xsupport"), QString("xnav")}) {
                auto* component = base ? base->findSubmodule(name) : nullptr;
                const bool ready = component && component->moduleState() == bwaf::BModule::STATE_IDLE;
                result[name] = ready;
                passed = passed && ready;
            }
            result["window"] = imol.wgt()->isVisible();
            result["xmenu"] = imol.wgt()->findChild<QWidget*>("XMenu") != nullptr;
            passed = passed && result["window"].toBool() && result["xmenu"].toBool();
            result["business_modules"] = imol.submoduleNames().size() - (base ? 1 : 0);
            QFile report("smoke-test.json");
            if (report.open(QIODevice::WriteOnly)) report.write(QJsonDocument(result).toJson());
            imol.wgt()->grab().save("smoke-test.png");
            a.exit(passed ? 0 : 2);
        });
    }

    return a.exec();
}
