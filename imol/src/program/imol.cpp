#include "imol.h"
#include "ui_imol.h"

#include "core/common.h"
#include "bwaf/bmodule.h"
#include "bwaf/bfactory.h"
#include "bwaf/bunit.h"

#include <QSplashScreen>
#include <QMainWindow>
#include <QGridLayout>
#include <QLibrary>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QHotkey>

#include "terminal.h"

#define IMOL_MIN_API 11

IMOL_RELEASE_VER_REG(IMOL_NAME, 10, 2, 66)

Imol::Imol(QSplashScreen *startup_screen, QObject *parent) : bwaf::BModule(parent),
    ui(new Ui::Imol),
#ifndef QT_NO_DEBUG
    m_startup_screen(startup_screen),
#endif
    m_main_window(new QMainWindow),
    m_hot_key(nullptr),
    m_plugin_root(this)
{
    bindMobj(IMOL_NAME);

    bool is_debug = mobj()->r("lic.debug")->getBool();

    int startup_msg_space_line = mobj()->r("lic.splash.space_line")->getInt();
    int startup_msg_align = mobj()->r("lic.splash.center")->getBool() ? Qt::AlignBottom | Qt::AlignHCenter : Qt::AlignBottom;
    QColor startup_msg_color = Qt::white;
    QString startup_msg_color_str = mobj()->r("lic.splash.color")->getString();
    if (!startup_msg_color_str.isEmpty()) startup_msg_color = QColor(startup_msg_color_str.toUInt(nullptr, 16));
#ifndef QT_NO_DEBUG
    m_startup_msg_space_line = startup_msg_space_line;
    m_startup_msg_align = startup_msg_align;
    m_startup_msg_color = startup_msg_color;
#endif
    splash(startup_screen, translator().dtr(IMOL_NAME, "Program starting..."), startup_msg_space_line, startup_msg_align, startup_msg_color);

    ui->setupUi(m_main_window);
    m_main_window->setWindowTitle(mobj()->r("lic.title")->getString(translator().dtr(IMOL_NAME, "Imol")));

    QWidget *main_wgt = new QWidget(m_main_window);
    main_wgt->setObjectName(IMOL_NAME);
    main_wgt->setLayout(new QGridLayout(main_wgt));
    main_wgt->layout()->setMargin(0);
    main_wgt->layout()->setSpacing(0);
    m_main_window->setCentralWidget(main_wgt);

    //bwaf ui initialization
    loadUI(mobj()->r("lic.ui"));

    ///
    /// start states
    ///
    /// Imol is the root BModule of whole program, runs states in sequence by itself.
    ///

    // plugin system
    imol::pluginManager().keyMobj()->watch(true);
    connect(imol::pluginManager().keyMobj(), &imol::ModuleObject::activated, this, &Imol::cachePlugin);

    // setup
    splash(startup_screen, translator().dtr(IMOL_NAME, "Loading submodules..."), startup_msg_space_line, startup_msg_align, startup_msg_color);
    this->startSetup();
#ifndef QT_NO_DEBUG
    if (is_debug) SLOG << "------------------------";
#endif
    // re-show startup splash screen
    if (startup_screen && startup_screen->pixmap().isNull()) {
        QString pixmap_path = mobj()->r("lic.splash.pixmap")->getString(DEFAULT_SPLASH_PIXMAP);
        QPixmap pixmap(pixmap_path);
        if (pixmap.isNull()) {
            ELOG << "<imol> cannot find splash screen pixmap: \"" << pixmap_path << "\"";
            pixmap = QPixmap(DEFAULT_SPLASH_PIXMAP);
        }
        if (mobj()->hasRmobj("lic.splash.size")) {
            QIcon icon(pixmap_path);
            QSize size(mobj()->r("lic.splash.size.w")->getInt(), mobj()->r("lic.splash.size.h")->getInt());
            startup_screen->setPixmap(icon.pixmap(size));
        } else {
            startup_screen->setPixmap(pixmap);
        }
        startup_screen->show();
    }

    // init
    splash(startup_screen, translator().dtr(IMOL_NAME, "Initializing interface elements..."), startup_msg_space_line, startup_msg_align, startup_msg_color);
    this->startInit();
#ifndef QT_NO_DEBUG
    if (is_debug) SLOG << "------------------------";
#endif

    // render
    splash(startup_screen, translator().dtr(IMOL_NAME, "Rendering..."), startup_msg_space_line, startup_msg_align, startup_msg_color);
    this->startRender();
#ifndef QT_NO_DEBUG
    if (is_debug) SLOG << "------------------------";
#endif

    // size option
    loadSize(mobj()->r("lic.size"));

    // idle
    splash(startup_screen, translator().dtr(IMOL_NAME, "Preparing content..."), startup_msg_space_line, startup_msg_align, startup_msg_color);
    this->startIdle();
#ifndef QT_NO_DEBUG
    if (is_debug) SLOG << "------------------------";

    m_startup_screen = nullptr;
#endif

    ///
    /// terminal
    ///
    if (is_debug) {
        QString hot_key_str = mobj()->r("lic.hot_key")->getString("Ctrl+Shift+t");
        if (!hot_key_str.isEmpty()) {
            m_hot_key = new QHotkey(QKeySequence(hot_key_str), true);

            Terminal::instance().close();
            connect(m_hot_key, &QHotkey::activated, this, [] {
                Terminal::instance().show();
                Terminal::instance().raise();
            });
        }
    }
}

Imol::~Imol()
{
    //detach
    startDetach();

    //deconstruct
    qDeleteAll(m_submodules);
    m_submodules.clear();

    //free memory
    delete m_hot_key;
    delete ui;
    delete m_main_window;

    m().cancel(this, IMOL_NAME);
}

QWidget * Imol::wgt()
{
    return m_main_window;
}

#ifndef QT_NO_DEBUG
QString getDebugModuleStateStr(bwaf::BModule::ModuleState state)
{
    switch (state) {
    case bwaf::BModule::STATE_SETUP: return "<setup>";
    case bwaf::BModule::STATE_INIT: return "<init>";
    case bwaf::BModule::STATE_RENDER: return "<render>";
    case bwaf::BModule::STATE_IDLE: return "<idle>";
    case bwaf::BModule::STATE_DETACH: return "<detach>";
    default: break;
    }
    return "";
}

QString getStartupStateStr(bwaf::BModule::ModuleState state, const QString &prefix, const QString &module_name)
{
    switch (state) {
    case bwaf::BModule::STATE_SETUP: return translator().dtr(IMOL_NAME, "Loading: %1%2...").arg(prefix, module_name);
    case bwaf::BModule::STATE_INIT: return translator().dtr(IMOL_NAME, "Initializing: %1%2...").arg(prefix, module_name);
    case bwaf::BModule::STATE_RENDER: return translator().dtr(IMOL_NAME, "Rendering: %1%2...").arg(prefix, module_name);
    case bwaf::BModule::STATE_IDLE: return translator().dtr(IMOL_NAME, "Preparing: %1%2...").arg(prefix, module_name);
    case bwaf::BModule::STATE_DETACH: return translator().dtr(IMOL_NAME, "Detaching: %1%2...").arg(prefix, module_name);
    default: break;
    }
    return "";
}

void Imol::regitstRecursiveModuleMsgDebug(bwaf::BModule *module, const QString &prefix)
{
    if (!module) {
        ELOG << prefix << "has NULL submodule!";
        return;
    }
    //connect this module
    QObject::connect(module, &bwaf::BModule::moduleStateAboutToChange, this, [module, prefix, this] (bwaf::BModule::ModuleState state) {
        SLOG << getDebugModuleStateStr(state) << prefix << module->name();
        splash(m_startup_screen, getStartupStateStr(state, prefix, module->name()), m_startup_msg_space_line, m_startup_msg_align, m_startup_msg_color);
    });
    //connect submodules
    foreach (bwaf::BModule *submodule, module->submodules()) {
        regitstRecursiveModuleMsgDebug(submodule, prefix + module->name() + ".");
    }
}
#endif

void Imol::afterSetup()
{
    loadExternalModules(mobj()->r("lic.module"));
    loadExternalModules(mobj()->r("cfg.module"));
#ifndef QT_NO_DEBUG
    if (!mobj()->r("lic.debug")->getBool()) return;

    foreach (bwaf::BModule *submodule, submodules()) {
        regitstRecursiveModuleMsgDebug(submodule);
    }
#endif
}

void addBwaf(QWidget *bwaf_wgt, QGridLayout *grid_layout, const QString &param)
{
    QStringList params = param.split(",");
    if (params.size() < 4) {
        ELOG << "parse error for bwaf param: " << param;
        delete bwaf_wgt;
        return;
    }

    int row = params.value(0, "").toInt();
    int column = params.value(1, "").toInt();
    int row_span = params.value(2, "").toInt();
    int column_span = params.value(3, "").toInt();
    grid_layout->addWidget(bwaf_wgt, row, column, row_span, column_span);
}

void Imol::splash(QSplashScreen *screen, const QString &msg, int space_line, int align, const QColor &color)
{
    if (!screen || !screen->isVisible()) return;

    screen->showMessage(msg + QByteArray(space_line, '\n'), align, color);
    qApp->processEvents();
}

void Imol::loadUI(imol::ModuleObject *ui_mobj)
{
    QGridLayout *gl = qobject_cast<QGridLayout *>(m_main_window->centralWidget()->layout());
    foreach (auto ui_item_mobj, ui_mobj->cmobjs()) {
        if (auto wgt = bwaf::bUnitCreator().create(ui_item_mobj->name(), m_main_window)) {
            addBwaf(wgt, gl, ui_item_mobj->getString());
        } else {
            ELOG << "<imol> load ui failed: " << ui_item_mobj->name();
        }
    }

    class UIEventFilter : public QObject
    {
    public:
        explicit UIEventFilter(imol::ModuleObject *mobj, QMainWindow *ui) : QObject(ui)
        {
            mobj->set(this, "ui.mainwindow.event", QVariant());
            this->mobj = mobj->r("ui.mainwindow.event");
            this->mobj->set(this, "close", QVariant());
            this->mobj->set(this, "resize", ui->size());
        }

        bool eventFilter(QObject *, QEvent *event)
        {
            if (event->type() == QEvent::Close) {
                mobj->c("close")->trigger();
            } else if (event->type() == QEvent::Resize) {
                mobj->c("resize")->set(this, dynamic_cast<QResizeEvent *>(event)->size());
            }
            return false;
        }

    private:
        imol::ModuleObject *mobj;
    };
    m_main_window->installEventFilter(new UIEventFilter(mobj(), m_main_window));
}

void Imol::loadSize(imol::ModuleObject *size_mobj)
{
    bool is_maximized = size_mobj->c("maximized")->getBool();
    bool is_windowed = size_mobj->c("windowed")->getBool(true);
    m_main_window->resize(size_mobj->c("width")->getInt(m_main_window->width()), size_mobj->c("height")->getInt(m_main_window->height()));
    if (!is_windowed) m_main_window->setWindowFlags(Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);
    if (is_maximized) m_main_window->setWindowState(Qt::WindowFullScreen);
}

void Imol::loadExternalModules(imol::ModuleObject *modules_mobj)
{
    auto plugin_root = m_plugin_root;
    foreach (imol::ModuleObject *module_mobj, modules_mobj->cmobjs()) {
        QString path = module_mobj->c("path")->getString();
#ifndef QT_NO_DEBUG
        splash(m_startup_screen, translator().dtr(IMOL_NAME, "Loading submodule: %1...").arg(path), m_startup_msg_space_line, m_startup_msg_align, m_startup_msg_color);
#endif

        // load plugin
        ILOG << "<imol> load plugin from " << path;

        m_plugin_cache.clear();
        QLibrary lib(path);
//        lib.setLoadHints(QLibrary::ResolveAllSymbolsHint);
        if (!lib.isLoaded() && !lib.load()) {
            ELOG << "<imol> load plugin from " << path << " failed, reason: " << lib.errorString();
            continue;
        }

        foreach (const QString &plugin_key, m_plugin_cache) {
            int api = imol::pluginManager().create(plugin_key);
            if (api < IMOL_MIN_API) {
                ELOG << "<imol> plugin " << plugin_key << " is no longer support (api " << api << " < " << IMOL_MIN_API << ")";
                continue;;
            }

            auto bm = bfactory().create(plugin_key, plugin_root);
            if (!bm) {
                ELOG << "<imol> plugin " << plugin_key << " creates failed";
                continue;
            }

            ILOG << "<imol> plugin " << plugin_key << " (api: " << api << ") created at " << plugin_root->name();

            // sync module
            plugin_root->addSubmodule(bm);
            bm->runToState(plugin_root->moduleState());
        }

        // load ui
        if (module_mobj->hasCmobj("ui")) loadUI(module_mobj->c("ui"));

        // load submodule
        if (module_mobj->hasCmobj("submodule")) {
            QString module_name = QFileInfo(path).baseName();
            if (auto new_plugin_root = plugin_root->findSubmodule(module_name)) m_plugin_root = new_plugin_root;
            loadExternalModules(module_mobj->c("submodule"));
            m_plugin_root = plugin_root;
        }
    }
}

void Imol::cachePlugin(imol::ModuleObject *changed_mobj, int type)
{
    if (type != imol::ModuleObject::MOBJ_CHANGE) return;
    m_plugin_cache.append(changed_mobj->fullName(imol::pluginManager().keyMobj()));
}
