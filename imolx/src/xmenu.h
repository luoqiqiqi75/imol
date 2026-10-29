#ifndef XMENU_H
#define XMENU_H

#include <QWidget>
#include <QLabel>
#include <QElapsedTimer>

#include "xhelper.h"

class QMainWindow;

namespace Ui { class XMenu; }
class XMenu : public QWidget
{
    Q_OBJECT

public:
    explicit XMenu(QWidget *parent = nullptr);
    ~XMenu();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void prepareSearching();
    void navToUrl();
    void minimizeWnd();
    void maximizeWnd();
    void closeWnd();
    void moveWnd();

private:
    Ui::XMenu *ui;
    QMainWindow *m_main_wgt;
    QWidget *m_last_wgt;
    QPoint m_pos, m_tar_pos;

    int m_dt;
    QTimer *m_t;
    QElapsedTimer m_p_et, m_r_et;
};

#endif // XMENU_H
