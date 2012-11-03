#include "ui/controls/titlecontrol.h"

#include "ui_titlecontrol.h"

TitleControl::TitleControl(QWidget* pParent): QWidget(pParent), pUi(new Ui::TitleControl) {
    pUi->setupUi(this);

    paClose = new QAction("&Close", this);
    paClose->setStatusTip("Close the window");
    paClose->setIcon(QIcon(":/res/resources/exit.png"));

    ptbToolBar = new QToolBar(this);
    ptbToolBar->setVisible(false);

    ptbSystemBar = new QToolBar(this);
    ptbSystemBar->addAction(paClose);

    QWidget* pwEmpty = new QWidget(this);
    pwEmpty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    pUi->plLayout->addWidget(ptbToolBar);
    pUi->plLayout->addWidget(pwEmpty);
    pUi->plLayout->addWidget(ptbSystemBar);
}

TitleControl::~TitleControl() {
    delete pUi;
}

QAction* TitleControl::CloseAction() {
    return paClose;
}

QToolBar* TitleControl::ToolBar() {
    return ptbToolBar;
}
