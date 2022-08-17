#include "custom/touchbutton.h"

#include <QEvent>
#include <QApplication>

TouchButton::TouchButton(QWidget *parent) : QPushButton(parent),
    m_is_triggered(false)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    installEventFilter(this);
}

bool TouchButton::eventFilter(QObject *obj, QEvent *e)
{
    switch (e->type()) {
    case QEvent::TouchBegin: case QEvent::MouseButtonPress:
        if (m_is_triggered) break;
        m_is_triggered = true;
        if (!isEnabled()) break;
        emit touchBegun();
        break;
    case QEvent::TouchEnd: case QEvent::TouchCancel: case QEvent::MouseButtonRelease:
    case QEvent::Leave: case QEvent::FocusOut:
        if (!m_is_triggered) break;
        m_is_triggered = false;
        if (!isEnabled()) break;
        emit touchEnded();
        break;
    case QEvent::EnabledChange:
        if (!m_is_triggered) break;
        m_is_triggered = false;
        if (isEnabled()) break;
        emit touchEnded();
        break;
    default: break;
    }
    return false;
}
