#include "custom/touchbutton.h"

#include <QEvent>

TouchButton::TouchButton(QWidget *parent) : QPushButton(parent),
    m_is_triggered(false)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    this->installEventFilter(this);
}

#include "imol/logmanager.h"

bool TouchButton::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::TouchBegin || e->type() == QEvent::MouseButtonPress) {
        if (m_is_triggered) return QPushButton::eventFilter(obj, e);
        if (isEnabled()) emit touchBegun();
        m_is_triggered = true;
    } else if (e->type() == QEvent::TouchEnd || e->type() == QEvent::TouchCancel || e->type() == QEvent::MouseButtonRelease
               || e->type() == QEvent::Leave) {
        if (!m_is_triggered) return QPushButton::eventFilter(obj, e);
        if (isEnabled()) emit touchEnded();
        m_is_triggered = false;
    } else if (e->type() == QEvent::EnabledChange) {
        if (!m_is_triggered) return QPushButton::eventFilter(obj, e);
        if (!isEnabled()) emit touchEnded();
        m_is_triggered = false;
    }
    return QPushButton::eventFilter(obj, e);
}
