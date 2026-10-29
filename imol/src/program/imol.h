#ifndef IMOL_H
#define IMOL_H

#include "bwaf/bmodule.h"

#include <QHash>
#ifndef QT_NO_DEBUG
#include <QColor>
#endif

#define DEFAULT_SPLASH_PIXMAP ":/imol/splash.png"
#define DEFAULT_SPLASH_PIXMAP_HIGH_DPI ":/imol/splash@2x.png"

#define IMOL_NAME "imol"

namespace Ui { class Imol; }

class QSplashScreen;
class QMainWindow;
class QHotkey;
//!
//! \brief The Imol class is the general entrance for all modules based on imol structure, which is
//! also the root module for whole running process
//!
//! \inherits bwaf::BModule
//!
class Imol : public bwaf::BModule
{
    Q_OBJECT

public:
    explicit Imol(QSplashScreen *startup_screen, QObject *parent = nullptr);
    ~Imol();

    QString name() const override {return IMOL_NAME;}
    QWidget * wgt() override;

private:
#ifndef QT_NO_DEBUG
    //!
    //! \brief regitstRecursiveModuleMsgDebug monitors the module state by signal stateAboutToChange
    //!
    void regitstRecursiveModuleMsgDebug(bwaf::BModule *module, const QString &prefix = "");
#endif

    void afterSetup() override;

    void splash(QSplashScreen *screen, const QString &msg, int space_line, int align, const QColor &color);

    void loadUI(imol::ModuleObject *ui_mobj);
    void loadSize(imol::ModuleObject *size_mobj);
    void loadExternalModules(imol::ModuleObject *modules_mobj);

private slots:
    void cachePlugin(imol::ModuleObject *changed_mobj, int type);

private:
    Ui::Imol *ui;
#ifndef QT_NO_DEBUG
    QSplashScreen *m_startup_screen;
    int m_startup_msg_space_line;
    int m_startup_msg_align;
    QColor m_startup_msg_color;
#endif
    QMainWindow *m_main_window;
    QHotkey *m_hot_key;

    bwaf::BModule *m_plugin_root;
    QStringList m_plugin_cache;
};

#endif // IMOL_H
