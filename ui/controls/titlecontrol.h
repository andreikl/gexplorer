#ifndef TITLE_CONTROL_H
#define TITLE_CONTROL_H

#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
class QToolBar;
QT_END_NAMESPACE

namespace Ui
{
    class TitleControl;
}

class TitleControl: public QWidget
{
    Q_OBJECT

public:
    explicit TitleControl(QWidget* parent);
    virtual ~TitleControl();

public:
    QAction* getCloseAction();
    QToolBar* getToolBar();

protected:
    QToolBar* tbSystemBar;
    QToolBar* tbToolBar;
    QAction* aClose;

private:
    Ui::TitleControl* ui;
};

#endif // TITLEBAR_H
