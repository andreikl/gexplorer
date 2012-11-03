#include <QtCore/QDebug>
#include <QtCore/QUrl>

#include "application.h"
#include "ui/controls/titlecontrol.h"

#include "ui/controls/cleancontrol.h"

#include "ui_cleancontrol.h"

CleanControl::CleanControl(QWidget* pParent): QWidget(pParent), pUi(new Ui::CleanControl) {
    pUi->setupUi(this);

    pTitle = new TitleControl(this);
    pUi->plLayout->addWidget(pTitle);

    pView = new QDeclarativeView();
    pView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // content configure
    pView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    pView->setSource(QUrl::fromLocalFile("qml/GalleryExplorer/Control.qml"));

    pUi->plLayout->addWidget(pView);
}

CleanControl::~CleanControl() {
    delete pUi;
}
