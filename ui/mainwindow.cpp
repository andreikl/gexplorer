#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include <QtWidgets/QSplitter>

#include "application.h"
#include "config.h"

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"

#include "ui/controls/customgallerycontrol.h"
#include "ui/controls/gallerycontrol.h"
#include "ui/controls/browsercontrol.h"
#include "ui/controls/titlecontrol.h"

#include "ui/windows/customgallerieswindow.h"
#include "ui/windows/keygallerieswindow.h"
#include "ui/windows/gallerieswindow.h"
#include "ui/windows/downloadswindow.h"

#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

QStatusBar* bar = NULL;

MainWindow::MainWindow(QWidget* pParent): QMainWindow(pParent), customGallery(NULL), gallery(NULL), browser(NULL), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dwGalleries = new GalleriesWindow(this);
    //dwGalleries->setFloating(true);
    addDockWidget(Qt::RightDockWidgetArea, dwGalleries);

    dwCustomGalleries = new CustomGalleriesWindow(this);
    //dwCustomGalleries->setFloating(true);
    addDockWidget(Qt::RightDockWidgetArea, dwCustomGalleries);

    dwKeys = new KeyGalleriesWindow(this);
    //dwKeys->setFloating(true);
    addDockWidget(Qt::LeftDockWidgetArea, dwKeys);

    dwDownloads = new DownloadsWindow(this);
    //dwDownloads->setFloating(true);
    addDockWidget(Qt::LeftDockWidgetArea, dwDownloads);

    bar = ui->sbStatusBar;

    splitter = new QSplitter(Qt::Vertical, this);
    ui->lLayout->addWidget(splitter);

    isCustomGallery = false;
    isGallery = false;
    isBrowser = false;

    ui->mViews->addAction(dwKeys->toggleViewAction());
    connect(dwKeys->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(keyGalleriesWindowEvent(bool)));
    ui->mViews->addAction(dwCustomGalleries->toggleViewAction());
    connect(dwCustomGalleries->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(customGalleriesWindowEvent(bool)));
    ui->mViews->addAction(dwGalleries->toggleViewAction());
    connect(dwGalleries->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(galleriesWindowEvent(bool)));
    ui->mViews->addAction(dwDownloads->toggleViewAction());
    connect(dwDownloads->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(downloadsWindowEvent(bool)));

    if(Config::getInstance()->getIsKeyGalleriesWindow()) {
        dwKeys->setVisible(true);
    } else {
        dwKeys->setVisible(false);
    }
    if(Config::getInstance()->getIsCustomGalleriesWindow()) {
        dwCustomGalleries->setVisible(true);
    } else {
        dwCustomGalleries->setVisible(false);
    }
    if(Config::getInstance()->getIsGalleriesWindow()) {
        dwGalleries->setVisible(true);
    } else {
        dwGalleries->setVisible(false);
    }
    if(Config::getInstance()->getIsDownloadsWindow()) {
        dwDownloads->setVisible(true);
    } else {
        dwDownloads->setVisible(false);
    }

    connect(CustomGalleryHandler::getInstance(), SIGNAL(onDelCustomGallery(CustomGalleryData*)), SLOT(delCustomGalleryEvent(CustomGalleryData*)));
    connect(GalleryHandler::getInstance(), SIGNAL(onDelGallery(GalleryData*)), SLOT(delGalleryEvent(GalleryData*)));

    connect(dwCustomGalleries, SIGNAL(onGallery(CustomGalleryData*)), SLOT(customGalleryEvent(CustomGalleryData*)));
    connect(dwCustomGalleries, SIGNAL(onGotoKey(KeyData*, CustomGalleryData*)), SLOT(gotoKeyEvent(KeyData*, CustomGalleryData*)));

    connect(dwGalleries, SIGNAL(onBrowse(GalleryData*)), SLOT(browseGalleryEvent(GalleryData*)));
    connect(dwGalleries, SIGNAL(onAdd(const QString&)), SLOT(addGalleryEvent(const QString&)));
    connect(dwGalleries, SIGNAL(onGallery(GalleryData*)), SLOT(galleryEvent(GalleryData*)));

    connect(dwKeys, SIGNAL(onGotoGallery(CustomGalleryData*)), SLOT(gotoCustomGalleryEvent(CustomGalleryData*)));
    connect(dwKeys, SIGNAL(onGallery(CustomGalleryData*)), SLOT(customGalleryEvent(CustomGalleryData*)));

    connect(ui->aExit, SIGNAL(triggered()), SLOT(close()));

    //"http://www.gettyimagesgallery.com/Collections/Archive/Patrick-Lichfield.aspx"
    //"http://wallpaper.metalship.org/en/pictures/Bands59"
    //"http://en.metalship.org/bands/Dimmu%20Borgir"
    //ui->pucBrowser->GoTo("http://en.metalship.org/bands/Dimmu%20Borgir");
}

MainWindow::~MainWindow()
{
    if(browser) {
        browser->close();
        delete browser;
        browser = NULL;
    }
    if(gallery) {
        gallery->close();
        delete gallery;
        gallery = NULL;
    }
    if(customGallery) {
        customGallery->close();
        delete customGallery;
        customGallery = NULL;
    }
    delete ui;
}

void MainWindow::gotoCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    dwCustomGalleries->setVisible(true);
    Config::getInstance()->setIsCustomGalleriesWindow(true);

    dwCustomGalleries->selectGallery(item->getCustomGallery());

    if(!isCustomGallery || customGallery->getGallery() != item->getCustomGallery()) {
        customGalleryEvent(item->getCustomGallery());
    }
    customGallery->selectItem(item);
}

void MainWindow::gotoCustomGalleryEvent(CustomGalleryData* value)
{
    dwCustomGalleries->setVisible(true);
    Config::getInstance()->setIsCustomGalleriesWindow(true);

    dwCustomGalleries->selectGallery(value);
}

void MainWindow::gotoGalleryItemEvent(GalleryItemData* item)
{
    dwGalleries->setVisible(true);
    Config::getInstance()->setIsGalleriesWindow(true);

    dwGalleries->selectGallery(item->getGallery());

    if(!isGallery || gallery->getGallery() != item->getGallery()) {
        galleryEvent(item->getGallery());
    }
    gallery->selectItem(item);
}

void MainWindow::gotoKeyEvent(KeyData* key, CustomGalleryData* gallery)
{
    dwKeys->setVisible(true);
    Config::getInstance()->setIsKeyGalleriesWindow(true);

    dwKeys->selectKey(key, gallery);
}

void MainWindow::customGalleryEvent(CustomGalleryData* value)
{
    if(customGallery == NULL) {
        customGallery = new CustomGalleryControl(this);
        connect(customGallery->getTitle()->getCloseAction(), SIGNAL(triggered()), SLOT(exitCustomGalleryEvent()));
        connect(customGallery, SIGNAL(onGotoItem(GalleryItemData*)), SLOT(gotoGalleryItemEvent(GalleryItemData*)));
    }
    if(!isCustomGallery) {
        splitter->addWidget(customGallery);
        isCustomGallery = true;
    }
    customGallery->setGallery(value);
}

void MainWindow::browseGalleryEvent(GalleryData* value)
{
    showBrowser();
    browser->goTo(value);
}

void MainWindow::galleryEvent(GalleryData* value)
{
    if(isBrowser) {
        browser->setParent(NULL);
        isBrowser = false;
    }
    if(gallery == NULL) {
        gallery = new GalleryControl(this);
        connect(gallery->getTitle()->getCloseAction(), SIGNAL(triggered()), SLOT(exitGalleryEvent()));
        connect(gallery, SIGNAL(onGotoItem(CustomGalleryItemData*)), SLOT(gotoCustomGalleryItemEvent(CustomGalleryItemData*)));
    }
    if(!isGallery) {
        if(isCustomGallery) {
            splitter->insertWidget(0, gallery);
        } else {
            splitter->addWidget(gallery);
        }
        isGallery = true;
    }
    gallery->setGallery(value);
}

void MainWindow::addGalleryEvent(const QString& source)
{
    if(source.startsWith("http://", Qt::CaseInsensitive)) {
        showBrowser();
        browser->goTo(source);
    } else {
        GalleryHandler::getInstance()->addFileGallery(source);
    }
}

void MainWindow::delCustomGalleryEvent(CustomGalleryData* gallery)
{
    if(this->customGallery && this->customGallery->getGallery() == gallery) {
        if(isCustomGallery) {
            this->customGallery->setParent(NULL);
            isCustomGallery = false;
        }
        this->customGallery->setGallery(NULL);
    }
}

void MainWindow::delGalleryEvent(GalleryData* gallery)
{
    if(this->gallery && this->gallery->getGallery() == gallery) {
        if(isGallery) {
            this->gallery->setParent(NULL);
            isGallery = false;
        }
        this->gallery->setGallery(NULL);
    }
    if(this->customGallery) {
        this->customGallery->setGallery(this->customGallery->getGallery());
    }
}

void MainWindow::customGalleriesWindowEvent(bool value)
{
    Config::getInstance()->setIsCustomGalleriesWindow(value);
}

void MainWindow::keyGalleriesWindowEvent(bool value)
{
    Config::getInstance()->setIsKeyGalleriesWindow(value);
}

void MainWindow::galleriesWindowEvent(bool value)
{
    Config::getInstance()->setIsGalleriesWindow(value);
}

void MainWindow::downloadsWindowEvent(bool value)
{
    Config::getInstance()->setIsDownloadsWindow(value);
}

void MainWindow::exitCustomGalleryEvent()
{
    customGallery->setParent(NULL);
    isCustomGallery = false;
}

void MainWindow::exitGalleryEvent()
{
    gallery->setParent(NULL);
    isGallery = false;
}

void MainWindow::exitBrowserEvent()
{
    browser->setParent(NULL);
    isBrowser = false;
}

void MainWindow::showBrowser()
{
    if(isGallery) {
        gallery->setParent(NULL);
        isGallery = false;
    }
    if(browser == NULL) {
        browser = new BrowserControl(this);
        connect(browser->getTitle()->getCloseAction(), SIGNAL(triggered()), SLOT(exitBrowserEvent()));
    }
    if(!isBrowser) {
        if(isCustomGallery) {
            splitter->insertWidget(0, browser);
        } else {
            splitter->addWidget(browser);
        }
        isBrowser = true;
    }
}
