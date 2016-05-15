#include <QtCore/QTimer>

#include <QtGui/QContextMenuEvent>

#include <QToolBar>
#include <QMenu>

#include "ui/common/gallerylistmodel.h"

#include "ui/controls/viewercontrol.h"
#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/viewerdialog.h"
#include "ui_viewerdialog.h"

ViewerDialog::ViewerDialog(QWidget* parent, GalleryListModel& model, int row): QDialog(parent), ui(new Ui::ViewerDialog)
{
    isSlide = false;

    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);

    title = new TitleControl(this);
    ui->lLayout->addWidget(title);

    viewer = new ViewerControl(this, model, row);
    viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->lLayout->addWidget(viewer);

    createMenuAndActions();
    updateButtons();

    //QTimer::singleShot(10, this, SLOT(currentEvent()));
    currentEvent();
}

ViewerDialog::~ViewerDialog()
{
    delete ui;
}

void ViewerDialog::contextMenuEvent(QContextMenuEvent* event)
{
    mMenu->exec(event->globalPos());
}

void ViewerDialog::currentEvent()
{
    viewer->currentEvent();
}

void ViewerDialog::slideEvent() {
    if(!isSlide) {
        aSlide->setText("&Stop slide show");
        aSlide->setStatusTip("Stop slide show");
        aSlide->setIcon(QIcon(":/res/resources/stop.png"));
        isSlide = true;

        QTimer::singleShot(3000, this, SLOT(tickEvent()));
    } else {
        aSlide->setText("&Start slide show");
        aSlide->setStatusTip("Start slide show");
        aSlide->setIcon(QIcon(":/res/resources/play.png"));
        isSlide = false;
    }
}

void ViewerDialog::closeEvent()
{
    close();
    delete this;
}

void ViewerDialog::backEvent()
{
    viewer->backEvent();
    updateButtons();
}

void ViewerDialog::nextEvent()
{
    viewer->nextEvent();
    updateButtons();
}

void ViewerDialog::tickEvent()
{
    const QModelIndex& i2 = viewer->getModel().getNextItem(viewer->getCurrentRow());
    if(isSlide && i2.isValid()) {
        nextEvent();

        QTimer::singleShot(3000, this, SLOT(tickEvent()));
    } else {
        aSlide->setText("&Start slide show");
        aSlide->setStatusTip("Start slide show");
        aSlide->setIcon(QIcon(":/res/resources/play.png"));
        isSlide = false;
    }
}

void ViewerDialog::createMenuAndActions()
{
    title->getToolBar()->setVisible(true);
    mMenu = new QMenu(this);

    aBack = new QAction("&Previus", this);
    aBack->setStatusTip("Show previus item");
    aBack->setIcon(QIcon(":/res/resources/back.png"));
    title->getToolBar()->addAction(aBack);
    mMenu->addAction(aBack);
    connect(aBack, SIGNAL(triggered()), this, SLOT(backEvent()));

    aNext = new QAction("&Next", this);
    aNext->setStatusTip("Show next item");
    aNext->setIcon(QIcon(":/res/resources/next.png"));
    title->getToolBar()->addAction(aNext);
    mMenu->addAction(aNext);
    connect(aNext, SIGNAL(triggered()), this, SLOT(nextEvent()));

    aSlide = new QAction("&Start slide show", this);
    aSlide->setStatusTip("Start slide show");
    aSlide->setIcon(QIcon(":/res/resources/play.png"));
    title->getToolBar()->addAction(aSlide);
    mMenu->addAction(aSlide);
    connect(aSlide, SIGNAL(triggered()), this, SLOT(slideEvent()));

    connect(title->getCloseAction(), SIGNAL(triggered()), this, SLOT(closeEvent()));
}

void ViewerDialog::updateButtons()
{
    const QModelIndex& i1 = viewer->getModel().getBackItem(viewer->getCurrentRow());
    if(i1.isValid()) {
        aBack->setEnabled(true);
    } else {
        aBack->setEnabled(false);
    }
    const QModelIndex& i2 = viewer->getModel().getNextItem(viewer->getCurrentRow());
    if(i2.isValid()) {
        aNext->setEnabled(true);
        aSlide->setEnabled(true);
    } else {
        aNext->setEnabled(false);
        aSlide->setEnabled(false);
    }
}
