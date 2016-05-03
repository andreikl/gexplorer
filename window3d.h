#ifndef WINDOW3D_H
#define WINDOW3D_H

#include <QtWidgets/QWidget>

class Window3D: public QWidget
{
    Q_OBJECT

public:
    explicit Window3D(QWidget* parent = NULL);
    explicit Window3D(QWindow* hostedWindow, QWidget* parent = NULL);

    void setHostedWindow(QWindow* window);
    QWindow* hostedWindow() const;
    void setVisible(bool visible);

protected:
    bool eventFilter(QObject* obj, QEvent* evt);
    bool event(QEvent* evt);

private:
    void setHostedWindowHelper(QWindow *window);
    void init();

private:
    QWindow* hostWindow;

};

#endif // WINDOW3D_H
