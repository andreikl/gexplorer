#include <QToolBar>

#include "ui/controls/titlecontrol.h"
#include "ui_titlecontrol.h"

TitleControl::TitleControl(QWidget* parent): QWidget(parent), ui(new Ui::TitleControl)
{
    ui->setupUi(this);

    aClose = new QAction("&Close", this);
    aClose->setStatusTip("Close the window");
    aClose->setIcon(QIcon(":/res/resources/exit.png"));

    tbToolBar = new QToolBar(this);
    tbToolBar->setVisible(false);

    tbSystemBar = new QToolBar(this);
    tbSystemBar->addAction(aClose);

    QWidget* wEmpty = new QWidget(this);
    wEmpty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    ui->lLayout->addWidget(tbToolBar);
    ui->lLayout->addWidget(wEmpty);
    ui->lLayout->addWidget(tbSystemBar);
}

TitleControl::~TitleControl()
{
    delete ui;
}

QAction* TitleControl::getCloseAction()
{
    return aClose;
}

QToolBar* TitleControl::getToolBar()
{
    return tbToolBar;
}
