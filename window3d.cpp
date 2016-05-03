#include <QtGui/QWindow>

#include "window3d.h"

Window3D::Window3D(QWidget* parent): QWidget(parent), hostWindow(NULL)
{
    init();
}

Window3D::Window3D(QWindow* window, QWidget* parent): QWidget(parent) , hostWindow(window)
{
    init();
}

void Window3D::init()
{
    //setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_NoBackground);
    setHostedWindowHelper(hostWindow);
}


void Window3D::setHostedWindowHelper(QWindow* window)
{
    if(hostWindow) {
        hostWindow->removeEventFilter(this);
    }

    hostWindow = window;
    if(hostWindow) {
        hostWindow->installEventFilter(this);
        //hostWindow->setWindowFlags(hostWindow->windowFlags() | Qt::FramelessWindowHint);
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        hostWindow->setParent(windowHandle());
        hostWindow->resize(size());
        hostWindow->setVisible(isVisible());
    }
}

QWindow* Window3D::hostedWindow() const
{
    return hostWindow;
}

void Window3D::setHostedWindow(QWindow* window)
{
    setHostedWindowHelper(window);
}

void Window3D::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if(hostWindow) {
        hostWindow->setVisible(visible);
    }
}

bool Window3D::event(QEvent* evt)
{
    bool ret = QWidget::event(evt);
    if(hostWindow) {

    //if(evt->type() == QEvent::UpdateRequest)
    //    return true;

        switch(evt->type()) {
        case QEvent::Wheel:
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            //qGuiApp->sendEvent(hostWindow, evt);
            break;

        case QEvent::Show:
            if(!hostWindow->parent()) {
                hostWindow->setParent(windowHandle());
            }
            break;

        case QEvent::Resize:
            hostWindow->resize(this->size());
            break;

        case QEvent::Move:
            break;

        case QEvent::Hide:
            windowHandle()->close();
            break;

        default:
            break;
        }
    }
    return ret;
}

bool Window3D::eventFilter(QObject* obj, QEvent* evt)
{
    if(obj == hostWindow) {
        if(this->size().isEmpty() && evt->type() == QEvent::Expose) {
            return true;
        }
    }
    return false;
}
