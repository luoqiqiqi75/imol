#include "xmenu.h"
#include "ui_xmenu.h"

#include "bwaf/bunit.h"
#include "bwaf/topmenu.h"

#include <QMouseEvent>
#include <QTimer>
#include <QMainWindow>
#include <QLabel>

#include "xsupport.h"

#define NAME "xmenu"

IMOL_VER_REG(mName(MODULE_NAME, NAME), 3)

IMOL_REGISTER_BUNIT(NAME, XMenu)

XMenu::XMenu(QWidget *parent) : QWidget(parent),
    ui(new Ui::XMenu),
    m_main_wgt(qobject_cast<QMainWindow *>(bModule("imol")->wgt())),
    m_last_wgt(nullptr),
    m_dt(m("imol.cfg.xmenu.move_interval")->getInt(15)),
    m_t(new QTimer(this))
{
    setAttribute(Qt::WA_StyledBackground);

    ui->setupUi(this);

    setStyleSheet(ix::xtheme::stylizeQss(styleSheet()));

    ui->btnBack->setIcon(QIcon(ix::xtheme::pixmap("direction.left")));
    ui->btnForward->setIcon(QIcon(ix::xtheme::pixmap("direction.right")));
    ui->btnRefresh->setIcon(QIcon(ix::xtheme::pixmap("refresh")));
    ui->btnSearch->setIcon(QIcon(ix::xtheme::pixmap("search")));
    ui->btnMinimize->setIcon(QIcon(ix::xtheme::pixmap("window.min")));
    ui->btnMaximize->setIcon(QIcon(ix::xtheme::pixmap("window.max")));
    ui->btnClose->setIcon(QIcon(ix::xtheme::pixmap("window.close")));

    bool is_windowed = m("imol.lic.size.windowed")->getBool(true);
    bool is_maximized = m("imol.lic.size.maximized")->getBool();

    // topmenu container
    auto topmenu = instantiatedTopMenu();
    if (!topmenu) topmenu = new bwaf::TopMenu(m_main_wgt);

//    auto icon_btn = new QPushButton(qApp->windowIcon(), "", topmenu);
//    icon_btn->setObjectName(mName(NAME, "icon_btn"));
//    icon_btn->setIconSize(QSize(16, 16));
//    connect(m_main_wgt, &QMainWindow::windowIconChanged, icon_btn, [=] (const QIcon &icon) {
//        icon_btn->setIcon(icon);
//    });
//    topmenu->addLeft(this, icon_btn);

    QLabel *title_lbl = new QLabel(topmenu);
    if (!is_windowed) {
#ifndef XSUPPORT_QT4
        connect(m_main_wgt, &QMainWindow::windowTitleChanged, title_lbl, &QLabel::setText);
#endif
    }
    title_lbl->setObjectName(mName(NAME, "title_lbl"));
    title_lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    topmenu->addRight(this, title_lbl);
    ui->stkWgt->addWidget(topmenu);
    ui->stkWgt->setCurrentWidget(topmenu);
    m_last_wgt = topmenu;

    // operations
    ui->btnForward->setVisible(false);
    ui->btnRefresh->setVisible(false);
    connect(ui->btnBack, SIGNAL(clicked()), m("xnav.back"), SLOT(trigger()));
    connect(ui->btnSearch, SIGNAL(clicked()), this, SLOT(prepareSearching()));
    connect(ui->edtNav, SIGNAL(returnPressed()), this, SLOT(navToUrl()));

    // window
    if (is_windowed || is_maximized) {
        // border buttons
        ui->btnMinimize->setVisible(false);
        ui->btnMaximize->setVisible(false);
        ui->btnClose->setVisible(false);
    } else {
        // border buttons
        connect(ui->btnMinimize, SIGNAL(clicked()), this, SLOT(minimizeWnd()));
        connect(ui->btnMaximize, SIGNAL(clicked()), this, SLOT(maximizeWnd()));
        connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(closeWnd()));

//        m_main_wgt->setAttribute(Qt::WA_TranslucentBackground);
        auto wgt = m_main_wgt->centralWidget();
        wgt->setStyleSheet(wgt->styleSheet() + "QWidget#imol{ border: 1px solid #444444; }");
        wgt->layout()->setMargin(1);
    }

//    m_t->setSingleShot(true);
    connect(m_t, SIGNAL(timeout()), this, SLOT(moveWnd()));
}

XMenu::~XMenu()
{
    delete ui;
}

void XMenu::mousePressEvent(QMouseEvent *event)
{
    if (event->type() == QMouseEvent::MouseButtonPress && (event->button() & Qt::LeftButton)) {
        m_pos = m_main_wgt->pos() - event->globalPos();
        m_p_et.restart();
    }
    QWidget::mousePressEvent(event);
}

void XMenu::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (!m_pos.isNull()) {
            if (m_main_wgt->isMaximized()) {
                int x = event->pos().x();
                int old_w = m_main_wgt->width();
                ui->btnMaximize->click();
                QCoreApplication::processEvents();
                int new_w = m_main_wgt->width();
                m_pos -= QPoint(x * (old_w - new_w) / old_w, 0);
            }
            m_tar_pos = m_pos + event->globalPos();
            if (m_p_et.hasExpired(m_dt)) {
                m_t->start(0);
            } else {
                m_t->start(m_dt);
            }
        }
    }
    QWidget::mouseMoveEvent(event);
}

void XMenu::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        m_pos = QPoint();
    }
    QWidget::mouseReleaseEvent(event);
}

void XMenu::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        m_pos = QPoint();
        ui->btnMaximize->click();
    }
    QWidget::mouseDoubleClickEvent(event);
}

void XMenu::prepareSearching()
{
    if (ui->stkWgt->currentWidget() == ui->wgtSearch) {
        ui->stkWgt->setCurrentWidget(m_last_wgt);
    } else {
        m_last_wgt = ui->stkWgt->currentWidget();
        ui->stkWgt->setCurrentWidget(ui->wgtSearch);
        ui->edtNav->setFocus();
    }
}

void XMenu::navToUrl()
{
    ix::xnav::switchTo(this, ui->edtNav->text());
    ui->stkWgt->setCurrentWidget(m_last_wgt);
    ui->edtNav->clear();
}

void XMenu::minimizeWnd()
{
     m_main_wgt->showMinimized();
}

void XMenu::maximizeWnd()
{
    if (m_main_wgt->isMaximized()) {
        ui->btnMaximize->setIcon(QIcon(ix::xtheme::pixmap("window.max")));
        m_main_wgt->showNormal();
    } else {
        ui->btnMaximize->setIcon(QIcon(ix::xtheme::pixmap("window.normal")));
        m_main_wgt->showMaximized();
    }
}

void XMenu::closeWnd()
{
    qApp->closeAllWindows();
}

void XMenu::moveWnd()
{
    m_main_wgt->move(m_tar_pos);
    m_p_et.restart();
    m_t->stop();
}
