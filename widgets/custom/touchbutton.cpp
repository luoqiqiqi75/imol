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
    QPushButton::eventFilter(obj, e);

    switch (e->type()) {
    case QEvent::TouchBegin: case QEvent::MouseButtonPress:
        if (m_is_triggered) return false;
        m_is_triggered = true;
        if (!isEnabled()) return false;
        emit touchBegun();
        break;
    case QEvent::TouchEnd: case QEvent::TouchCancel: case QEvent::MouseButtonRelease: case QEvent::Leave:
        if (!m_is_triggered) return false;
        m_is_triggered = false;
        if (!isEnabled()) return false;
        emit touchEnded();
        break;
    case QEvent::EnabledChange:
        if (!m_is_triggered) return false;
        m_is_triggered = false;
        if (isEnabled()) return false;
        emit touchEnded();
        break;
    default: return false;
    }
    return true;
}
