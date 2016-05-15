#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <QFileDialog>
#include <QSplitter>

//#include <Qt3D/QGLAbstractScene>

#include "application.h"
#include "config.h"

//#include "viewer3d.h"
//#include "window3d.h"

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

#include "ui/dialogs/editworkspacedialog.h"

#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

QStatusBar* bar = NULL;

MainWindow::MainWindow(QWidget* pParent): QMainWindow(pParent), dwGalleries(NULL), dwCustomGalleries(NULL), dwKeys(NULL), dwDownloads(NULL),
    gallery(NULL), browser(NULL), isCustomGallery(false), isGallery(false), isBrowser(false), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->aWorkspace, SIGNAL(triggered()), this, SLOT(editWorkspaceEvent()));
    connect(ui->aLoadModel, SIGNAL(triggered()), this, SLOT(loadModelEvent()));

    bar = ui->sbStatusBar;

    splitter = new QSplitter(Qt::Vertical, this);
    ui->lLayout->addWidget(splitter);

    connect(CustomGalleryHandler::getInstance(), SIGNAL(onDelCustomGallery(CustomGalleryData*)), SLOT(delCustomGalleryEvent(CustomGalleryData*)));
    connect(GalleryHandler::getInstance(), SIGNAL(onDelGallery(GalleryData*)), SLOT(delGalleryEvent(GalleryData*)));
    connect(ui->aExit, SIGNAL(triggered()), SLOT(close()));

    loadWindows();

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
    if(CustomGalleryControl::getInstance()) {
        CustomGalleryControl::getInstance()->close();
        delete CustomGalleryControl::getInstance();
    }
    delete ui;
}

void MainWindow::editWorkspaceEvent()
{
    QString workspace;
    EditWorkspaceDialog* dialog = new EditWorkspaceDialog(NULL);
    dialog->show();
    dialog->exec();

    if(dialog->result() == QDialog::Accepted) {
        workspace = dialog->getWorkspace();
    }
    delete dialog;

    if(workspace.length() > 0 && QDir(workspace).exists()) {
        unloadWindows();

        if(Application::getInstance()) {
            delete Application::getInstance();
        }

        Config::getInstance()->setWorkspacePath(workspace);
        QDir::setCurrent(workspace);

        Application::createInstance(workspace);

        loadWindows();
    }
}

void MainWindow::loadModelEvent()
{
    //Window3D* window3D = new Window3D(new Viewer3D(), this);
    //splitter->addWidget(window3D);

    /*QString filter = QString("Models (%1)").arg(QGLAbstractScene::supportedFormats().join(" "));
    QString fileName = QFileDialog::getOpenFileName(NULL, QString("Open File"), QDir::currentPath(), filter);
    qDebug() << fileName;

    Viewer3D* viewer = new Viewer3D(fileName);
    viewer->resize(800, 600);
    viewer->show();*/
}

void MainWindow::gotoCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    dwCustomGalleries->setVisible(true);
    Config::getInstance()->setIsCustomGalleriesWindow(true);

    dwCustomGalleries->selectGallery(item->getCustomGallery());

    if(!isCustomGallery || CustomGalleryControl::getInstance()->getGallery() != item->getCustomGallery()) {
        customGalleryEvent(item->getCustomGallery());
    }
    CustomGalleryControl::getInstance()->selectItem(item);
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
    if(CustomGalleryControl::getInstance() == NULL) {
        CustomGalleryControl::createInstance(this);
        connect(CustomGalleryControl::getInstance()->getTitle()->getCloseAction(), SIGNAL(triggered()), SLOT(exitCustomGalleryEvent()));
        connect(CustomGalleryControl::getInstance(), SIGNAL(onGotoItem(GalleryItemData*)), SLOT(gotoGalleryItemEvent(GalleryItemData*)));
    }
    if(!isCustomGallery) {
        splitter->addWidget(CustomGalleryControl::getInstance());
        isCustomGallery = true;
    }
    CustomGalleryControl::getInstance()->setGallery(value);
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
    if(CustomGalleryControl::getInstance() && CustomGalleryControl::getInstance()->getGallery() == gallery) {
        if(isCustomGallery) {
            CustomGalleryControl::getInstance()->setParent(NULL);
            isCustomGallery = false;
        }
        CustomGalleryControl::getInstance()->setGallery(NULL);
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
    /*if(CustomGalleryControl::getInstance()) {
        CustomGalleryControl::getInstance()->setGallery(CustomGalleryControl::getInstance()->getGallery());
    }*/
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
    CustomGalleryControl::getInstance()->setParent(NULL);
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

void MainWindow::unloadWindows()
{
    ui->mViews->clear();

    if(CustomGalleryControl::getInstance()) {
        if(isCustomGallery) {
            CustomGalleryControl::getInstance()->setParent(NULL);
            isCustomGallery = false;
        }
        CustomGalleryControl::getInstance()->setGallery(NULL);
    }

    if(this->gallery) {
        if(isGallery) {
            this->gallery->setParent(NULL);
            isGallery = false;
        }
        this->gallery->setGallery(NULL);
    }

    if(this->browser) {
        if(isBrowser) {
            this->browser->setParent(NULL);
            isBrowser = false;
        }
        this->browser->goTo(NULL);
    }

    if(dwDownloads) {
        delete dwDownloads;
        dwDownloads = NULL;
    }

    if(dwKeys) {
        delete dwKeys;
        dwKeys = NULL;
    }

    if(dwCustomGalleries) {
        delete dwCustomGalleries;
        dwCustomGalleries = NULL;
    }

    if(dwGalleries) {
        delete dwGalleries;
        dwGalleries = NULL;
    }
}

void MainWindow::loadWindows()
{
    if(!dwGalleries)
    {
        dwGalleries = new GalleriesWindow(this);
        //dwGalleries->setFloating(true);
        addDockWidget(Qt::RightDockWidgetArea, dwGalleries);

        ui->mViews->addAction(dwGalleries->toggleViewAction());
        if(Config::getInstance()->getIsGalleriesWindow()) {
            dwGalleries->setVisible(true);
        } else {
            dwGalleries->setVisible(false);
        }

        connect(dwGalleries->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(galleriesWindowEvent(bool)));
        connect(dwGalleries, SIGNAL(onBrowse(GalleryData*)), SLOT(browseGalleryEvent(GalleryData*)));
        connect(dwGalleries, SIGNAL(onAdd(const QString&)), SLOT(addGalleryEvent(const QString&)));
        connect(dwGalleries, SIGNAL(onGallery(GalleryData*)), SLOT(galleryEvent(GalleryData*)));
    }

    if(!dwCustomGalleries)
    {
        dwCustomGalleries = new CustomGalleriesWindow(this);
        //dwCustomGalleries->setFloating(true);
        addDockWidget(Qt::RightDockWidgetArea, dwCustomGalleries);

        ui->mViews->addAction(dwCustomGalleries->toggleViewAction());
        if(Config::getInstance()->getIsCustomGalleriesWindow()) {
            dwCustomGalleries->setVisible(true);
        } else {
            dwCustomGalleries->setVisible(false);
        }

        connect(dwCustomGalleries->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(customGalleriesWindowEvent(bool)));
        connect(dwCustomGalleries, SIGNAL(onGallery(CustomGalleryData*)), SLOT(customGalleryEvent(CustomGalleryData*)));
        connect(dwCustomGalleries, SIGNAL(onGotoKey(KeyData*, CustomGalleryData*)), SLOT(gotoKeyEvent(KeyData*, CustomGalleryData*)));
    }

    if(!dwKeys)
    {
        dwKeys = new KeyGalleriesWindow(this);
        //dwKeys->setFloating(true);
        addDockWidget(Qt::LeftDockWidgetArea, dwKeys);

        ui->mViews->addAction(dwKeys->toggleViewAction());
        if(Config::getInstance()->getIsKeyGalleriesWindow()) {
            dwKeys->setVisible(true);
        } else {
            dwKeys->setVisible(false);
        }

        connect(dwKeys->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(keyGalleriesWindowEvent(bool)));
        connect(dwKeys, SIGNAL(onGotoGallery(CustomGalleryData*)), SLOT(gotoCustomGalleryEvent(CustomGalleryData*)));
        connect(dwKeys, SIGNAL(onGallery(CustomGalleryData*)), SLOT(customGalleryEvent(CustomGalleryData*)));
    }

    if(!dwDownloads)
    {
        dwDownloads = new DownloadsWindow(this);
        //dwDownloads->setFloating(true);
        addDockWidget(Qt::LeftDockWidgetArea, dwDownloads);

        ui->mViews->addAction(dwDownloads->toggleViewAction());

        if(Config::getInstance()->getIsDownloadsWindow()) {
            dwDownloads->setVisible(true);
        } else {
            dwDownloads->setVisible(false);
        }
        connect(dwDownloads->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(downloadsWindowEvent(bool)));
    }
}
