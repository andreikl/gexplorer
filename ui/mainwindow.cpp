#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include "config.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"

#include "ui/controls/titlecontrol.h"

#include "ui/mainwindow.h"

#include "ui_mainwindow.h"

QStatusBar* pBar = NULL;

MainWindow::MainWindow(QWidget* pParent): QMainWindow(pParent), pBrowser(NULL), pGallery(NULL), pCustomGallery(NULL), pClean(NULL), pUi(new Ui::MainWindow) {
    pUi->setupUi(this);

    pBar = pUi->psbStatusBar;

    pClean = new CleanControl();
    pUi->plLayout->addWidget(pClean);

    isBrowser = false;
    isGallery = false;
    isCustomGallery = false;

    pUi->pmViews->addAction(pUi->pdwKeyGalleriesWindow->toggleViewAction());
    connect(pUi->pdwKeyGalleriesWindow->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(keyGalleriesWindowEvent(bool)));
    pUi->pmViews->addAction(pUi->pdwCustomGalleriesWindow->toggleViewAction());
    connect(pUi->pdwCustomGalleriesWindow->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(customGalleriesWindowEvent(bool)));
    pUi->pmViews->addAction(pUi->pdwGalleriesWindow->toggleViewAction());
    connect(pUi->pdwGalleriesWindow->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(galleriesWindowEvent(bool)));
    pUi->pmViews->addAction(pUi->pdwDownloadsWindow->toggleViewAction());
    connect(pUi->pdwDownloadsWindow->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(downloadsWindowEvent(bool)));

    if(Config::Self()->IsKeyGalleriesWindow()) {
        pUi->pdwKeyGalleriesWindow->setVisible(true);
    } else {
        pUi->pdwKeyGalleriesWindow->setVisible(false);
    }
    if(Config::Self()->IsCustomGalleriesWindow()) {
        pUi->pdwCustomGalleriesWindow->setVisible(true);
    } else {
        pUi->pdwCustomGalleriesWindow->setVisible(false);
    }
    if(Config::Self()->IsGalleriesWindow()) {
        pUi->pdwGalleriesWindow->setVisible(true);
    } else {
        pUi->pdwGalleriesWindow->setVisible(false);
    }
    if(Config::Self()->IsDownloadsWindow()) {
        pUi->pdwDownloadsWindow->setVisible(true);
    } else {
        pUi->pdwDownloadsWindow->setVisible(false);
    }

    connect(GalleryHandler::getInstance(), SIGNAL(onDelGallery(GalleryData*)), SLOT(delGalleryEvent(GalleryData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onDelCustomGallery(CustomGalleryData*)), SLOT(delCustomGalleryEvent(CustomGalleryData*)));

    connect(pUi->pdwGalleriesWindow, SIGNAL(OnAdd(const QString&)), SLOT(AddGalleryEvent(const QString&)));
    connect(pUi->pdwGalleriesWindow, SIGNAL(OnBrowse(GalleryData*)), SLOT(BrowseGalleryEvent(GalleryData*)));
    connect(pUi->pdwGalleriesWindow, SIGNAL(OnGallery(GalleryData*)), SLOT(GalleryEvent(GalleryData*)));
    connect(pUi->pdwCustomGalleriesWindow, SIGNAL(OnGallery(CustomGalleryData*)), SLOT(customGalleryEvent(CustomGalleryData*)));

    connect(pUi->pdwKeyGalleriesWindow, SIGNAL(onGallery(CustomGalleryData*)), SLOT(customGalleryEvent(CustomGalleryData*)));
    connect(pUi->pdwKeyGalleriesWindow, SIGNAL(onGotoGallery(CustomGalleryData*)), SLOT(gotoCustomGalleryEvent(CustomGalleryData*)));

    connect(pUi->paExit, SIGNAL(triggered()), SLOT(close()));

    //QTimer::singleShot(500, this, SLOT(InitEvent()));
    //"http://www.gettyimagesgallery.com/Collections/Archive/Patrick-Lichfield.aspx"
    //"http://wallpaper.metalship.org/en/pictures/Bands59"
    //"http://en.metalship.org/bands/Dimmu%20Borgir"
    //pUi->pucBrowser->GoTo("http://en.metalship.org/bands/Dimmu%20Borgir");
}

MainWindow::~MainWindow() {
    if(pClean) {
        pClean->close();
        delete pClean;
        pClean = NULL;
    }
    if(pBrowser) {
        pBrowser->close();
        delete pBrowser;
        pBrowser = NULL;
    }
    if(pGallery) {
        pGallery->close();
        delete pGallery;
        pGallery = NULL;
    }
    if(pCustomGallery) {
        pCustomGallery->close();
        delete pCustomGallery;
        pCustomGallery = NULL;
    }
    delete pUi;
}

void MainWindow::customGalleriesWindowEvent(bool value) {
    Config::Self()->IsCustomGalleriesWindow(value);
}

void MainWindow::keyGalleriesWindowEvent(bool value) {
    Config::Self()->IsKeyGalleriesWindow(value);
}

void MainWindow::galleriesWindowEvent(bool value) {
    Config::Self()->IsGalleriesWindow(value);
}

void MainWindow::downloadsWindowEvent(bool value) {
    Config::Self()->IsDownloadsWindow(value);
}

void MainWindow::gotoCustomGalleryEvent(CustomGalleryData* value) {
    pUi->pdwCustomGalleriesWindow->setVisible(true);
    Config::Self()->IsCustomGalleriesWindow(true);

    pUi->pdwCustomGalleriesWindow->selectGallery(value);
}

void MainWindow::customGalleryEvent(CustomGalleryData* value) {
    pUi->plLayout->removeWidget(pClean);
    pClean->setParent(NULL);
    if(pCustomGallery == NULL) {
        pCustomGallery = new CustomGalleryControl();
        connect(pCustomGallery->Title()->CloseAction(), SIGNAL(triggered()), SLOT(ExitCustomGalleryEvent()));
    }
    if(!isCustomGallery) {
        pUi->plLayout->addWidget(pCustomGallery);
        isCustomGallery = true;
    }
    pCustomGallery->Gallery(value);
}

void MainWindow::delCustomGalleryEvent(CustomGalleryData* gallery)
{
    if(this->pCustomGallery && this->pCustomGallery->Gallery() == gallery) {
        if(isCustomGallery) {
            pUi->plLayout->removeWidget(this->pCustomGallery);
            this->pCustomGallery->setParent(NULL);
            isCustomGallery = false;
        }
        this->pCustomGallery->Gallery(NULL);
    }
}

void MainWindow::delGalleryEvent(GalleryData* gallery)
{
    if(this->pGallery && this->pGallery->Gallery() == gallery) {
        if(isGallery) {
            pUi->plLayout->removeWidget(this->pGallery);
            this->pGallery->setParent(NULL);
            isGallery = false;
        }
        this->pGallery->Gallery(NULL);
    }
    if(this->pCustomGallery) {
        this->pCustomGallery->Gallery(this->pCustomGallery->Gallery());
    }
}

void MainWindow::AddGalleryEvent(const QString& source) {
    if(source.startsWith("http://", Qt::CaseInsensitive)) {
        pUi->plLayout->removeWidget(pClean);
        pClean->setParent(NULL);
        if(isGallery) {
            pUi->plLayout->removeWidget(pGallery);
            pGallery->setParent(NULL);
            isGallery = false;
        }
        if(pBrowser == NULL) {
            pBrowser = new BrowserControl();
            connect(pBrowser->Title()->CloseAction(), SIGNAL(triggered()), SLOT(ExitBrowserEvent()));
        }
        if(!isBrowser) {
            if(isCustomGallery) {
                pUi->plLayout->insertWidget(0, pBrowser);
            } else {
                pUi->plLayout->addWidget(pBrowser);
            }
            isBrowser = true;
        }
        pBrowser->GoTo(source);
    } else {
        GalleryHandler::getInstance()->addFileGallery(source);
    }
}

void MainWindow::BrowseGalleryEvent(GalleryData* pValue) {
    pUi->plLayout->removeWidget(pClean);
    pClean->setParent(NULL);
    if(isGallery) {
        pUi->plLayout->removeWidget(pGallery);
        pGallery->setParent(NULL);
        isGallery = false;
    }
    if(pBrowser == NULL) {
        pBrowser = new BrowserControl();
        connect(pBrowser->Title()->CloseAction(), SIGNAL(triggered()), SLOT(ExitBrowserEvent()));
    }
    if(!isBrowser) {
        if(isCustomGallery) {
            pUi->plLayout->insertWidget(0, pBrowser);
        } else {
            pUi->plLayout->addWidget(pBrowser);
        }
        isBrowser = true;
    }
    pBrowser->GoTo(pValue);
}

void MainWindow::GalleryEvent(GalleryData* pValue) {
    pUi->plLayout->removeWidget(pClean);
    pClean->setParent(NULL);
    if(isBrowser) {
        pUi->plLayout->removeWidget(pBrowser);
        pBrowser->setParent(NULL);
        isBrowser = false;
    }
    if(pGallery == NULL) {
        pGallery = new GalleryControl();
        connect(pGallery->Title()->CloseAction(), SIGNAL(triggered()), SLOT(ExitGalleryEvent()));
    }
    if(!isGallery) {
        if(isCustomGallery) {
            pUi->plLayout->insertWidget(0, pGallery);
        } else {
            pUi->plLayout->addWidget(pGallery);
        }
        isGallery = true;
    }
    pGallery->Gallery(pValue);
}

void MainWindow::ExitBrowserEvent() {
    pUi->plLayout->removeWidget(pBrowser);
    pBrowser->setParent(NULL);
    isBrowser = false;
    pUi->plLayout->addWidget(pClean);
}

void MainWindow::ExitGalleryEvent() {
    pUi->plLayout->removeWidget(pGallery);
    pGallery->setParent(NULL);
    isGallery = false;
    if(!isCustomGallery) {
        pUi->plLayout->addWidget(pClean);
    }
}

void MainWindow::ExitCustomGalleryEvent() {
    pUi->plLayout->removeWidget(pCustomGallery);
    pCustomGallery->setParent(NULL);
    isCustomGallery = false;
    if(!isGallery) {
        pUi->plLayout->addWidget(pClean);
    }
}
