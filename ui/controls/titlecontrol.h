#ifndef TITLE_CONTROL_H
#define TITLE_CONTROL_H

#include <QtGui/QToolBar>
#include <QtGui/QWidget>

namespace Ui {
    class TitleControl;
}

class TitleControl: public QWidget {
    Q_OBJECT

public:
    explicit TitleControl(QWidget* pParent);
    virtual ~TitleControl();

public:
    QAction* CloseAction();
    QToolBar* ToolBar();

protected:
    QToolBar* ptbSystemBar;
    QToolBar* ptbToolBar;

private:
    QAction* paClose;

private:
    Ui::TitleControl* pUi;

};

#endif // TITLEBAR_H
