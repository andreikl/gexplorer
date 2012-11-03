#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QDir>

#include <QtGui/QMouseEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QMenu>

#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>

#include "application.h"

#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/galleryhandler.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/downloaddialog.h"
#include "ui/dialogs/searchdialog.h"

#include "ui/controls/browsercontrol.h"
#include "ui_browsercontrol.h"

extern QStatusBar* pBar;

BrowserView::BrowserView(QWidget* pWidget): QWebView(pWidget) {
    connect(this, SIGNAL(loadStarted()), this, SLOT(LoadStartedEvent()));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(LoadFinishedEvent()));
}

BrowserView::~BrowserView() {
}

const QString& BrowserView::TopHref() {
    return topHref;
}

void BrowserView::mouseMoveEvent(QMouseEvent* pEvent) {
    QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);

    QPoint point = page()->mainFrame()->scrollPosition();
    int x = pMouseEvent->x() + point.x();
    int y = pMouseEvent->y() + point.y();

    int i = 0;
    for(; i < hrefs.length(); i++) {
        const BrowserHrefItem& item = hrefs.at(i);
        if(item.rect.left() <= x && item.rect.right() >= x && item.rect.top() <= y && item.rect.bottom() >= y) {
            if(item.url != topHref) {
                topHref = item.url;

                const QUrl& u = url();
                pBar->showMessage(u.resolved(QUrl(topHref)).toString());
                emit OnTopHref();
            }
            break;
        }
    }
    if(i == hrefs.length()) {
        if(!topHref.isEmpty()) {
            topHref = "";

            pBar->showMessage("");
            emit OnTopHref();
        }
    }
    return QWebView::mouseMoveEvent(pEvent);
}

void BrowserView::LoadStartedEvent() {
    hrefs.clear();
}

void BrowserView::LoadFinishedEvent() {
    const QWebElement& element = page()->mainFrame()->documentElement();
    foreach(const QWebElement& e, element.findAll("[href]")) {
        const QWebElement& lc = e.lastChild();
        QRect r = e.geometry();

        const QRect& lcr = lc.geometry();
        r.setRight((lcr.right() > r.right()) ? lcr.right(): r.right());
        r.setBottom((lcr.bottom() > r.bottom()) ? lcr.bottom(): r.bottom());

        hrefs.append(BrowserHrefItem(r, e.attribute("href", "")));
    }
}

BrowserControl::BrowserControl(QWidget* pParent): QWidget(pParent), pTitle(NULL), pUi(new Ui::BrowserControl) {
    pUi->setupUi(this);

    pGallery = NULL;

    pTitle = new TitleControl(this);
    pUi->plLayout->addWidget(pTitle);

    pWeb = new BrowserView(this);
    pWeb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pUi->plLayout->addWidget(pWeb);

    pWeb->installEventFilter(this);
    pWeb->setContextMenuPolicy(Qt::NoContextMenu);

    //pWeb->settings()->setAttribute(QWebSettings::AutoLoadImages, false);
    pWeb->settings()->setAttribute(QWebSettings::JavaEnabled, false);
    pWeb->settings()->setAttribute(QWebSettings::PluginsEnabled, false);
    pWeb->settings()->setAttribute(QWebSettings::JavascriptEnabled, false);

    connect(pWeb, SIGNAL(OnTopHref()), this, SLOT(TopHrefEvent()));

    CreateMenuAndActions();
    UpdateButtons();
}

BrowserControl::~BrowserControl() {
    delete pUi;
}

TitleControl* BrowserControl::Title() {
    return pTitle;
}

void BrowserControl::GoTo(GalleryData* pGallery) {
    this->pGallery = pGallery;
    pWeb->load(QUrl(pGallery->getSource()));
    UpdateButtons();
}

void BrowserControl::GoTo(const QString& url) {
    this->pGallery = NULL;
    pWeb->load(QUrl(url));
    UpdateButtons();
}

bool BrowserControl::eventFilter(QObject* pObject, QEvent* pEvent) {
    bool isHandled = false;

    if(pGallery && pEvent->type() == QEvent::MouseButtonPress) {
        isHandled = true;
    }
    if(!isHandled) {
        return QWidget::eventFilter(pObject, pEvent);
    }
    return true;
}

void BrowserControl::contextMenuEvent(QContextMenuEvent* pEvent) {
    pmMenu->exec(pEvent->globalPos());
}

void BrowserControl::AddGalleryEvent() {
    GalleryData* pData = new GalleryData(GalleryData::WebGallery);
    const QUrl& url = pWeb->url();

    pData->setSource(url.toString());

    QString urlPath;
    QString urlExtension;
    ParseUrl(pWeb->TopHref(), urlPath, urlExtension);

    SearchDialog* pSearch = new SearchDialog(this, url.resolved(QUrl(pWeb->TopHref())).toString(), urlPath, urlExtension);
    pSearch->show();
    pSearch->exec();
    if(pSearch->result() == QDialog::Accepted) {
        urlPath = pSearch->Path();
        urlExtension = pSearch->Extension();
        QWebElement element = pWeb->page()->mainFrame()->documentElement();
        SearchSameUrls(element, urlPath, urlExtension, pData);
    }
    delete pSearch;

    if(Application::getInstance()->getExtensionByName(urlExtension) != NULL) {
        if(urlPath.isEmpty()) {
            urlPath = "./";
        }

        QString p = url.host() + url.path();
        QString u = url.resolved(QUrl(urlPath)).toString();

        pData->setPath(p);
        foreach(GalleryItemData* pItem, pData->getItems()) {
            pItem->setUrl(u + pItem->getFileName());
            pItem->setPath(QDir("").path());
        }

        DownloadDialog* pDownload = new DownloadDialog(this, *pData);
        pDownload->show();
        pDownload->exec();

        if(pDownload->result() == QDialog::Accepted) {
            pData->setPath(pDownload->Path());
            GalleryHandler::getInstance()->addWebGallery(*pData);
        }
        delete pDownload;
    }
    delete pData;
}

void BrowserControl::AddItemsEvent() {
    GalleryData* pData = new GalleryData(GalleryData::WebGallery);
    const QUrl& url = pWeb->url();

    pData->setSource(url.toString());

    QString urlPath;
    QString urlExtension;
    ParseUrl(pWeb->TopHref(), urlPath, urlExtension);

    SearchDialog* pSearch = new SearchDialog(this, url.resolved(QUrl(pWeb->TopHref())).toString(), urlPath, urlExtension);
    pSearch->show();
    pSearch->exec();
    if(pSearch->result() == QDialog::Accepted) {
        urlPath = pSearch->Path();
        urlExtension = pSearch->Extension();
        QWebElement element = pWeb->page()->mainFrame()->documentElement();
        SearchSameUrls(element, urlPath, urlExtension, pData);
    }
    delete pSearch;

    if(Application::getInstance()->getExtensionByName(urlExtension) != NULL) {
        if(urlPath.isEmpty()) {
            urlPath = "./";
        }

        QString p = url.host() + url.path();
        QString u = url.resolved(QUrl(urlPath)).toString();

        pData->setPath(p);
        foreach(GalleryItemData* pItem, pData->getItems()) {
            pItem->setUrl(u + pItem->getFileName());
            pItem->setPath(QDir("").path());
        }

        DownloadDialog* pDownload = new DownloadDialog(this, *pData);
        pDownload->show();
        pDownload->exec();

        if(pDownload->result() == QDialog::Accepted) {
            pData->setPath(pDownload->Path());
            //Application::Self()->AddToWebGallery(*pData);
        }
        delete pDownload;
    }
    delete pData;
}

void BrowserControl::TopHrefEvent() {
    UpdateButtons();
}

void BrowserControl::SearchSameUrls(QWebElement& element, const QString& path, const QString& ext, GalleryData* pData) {
    foreach(QWebElement e, element.findAll("[href]")) {
        QString url = e.attribute("href");
        if(url.startsWith(path, Qt::CaseInsensitive) && url.endsWith(ext, Qt::CaseInsensitive)) {
            QString midle = url.mid(path.length(), url.length() - path.length() - ext.length());
            if(midle.contains("/")) {
                continue;
            }
            QString file = url.right(url.length() - path.length());
            if(!IsContainsFile(pData, file)) {
                GalleryItemData* pItem = new GalleryItemData(pData, *Application::getInstance()->getExtensionByName(ext));
                pItem->setFileName(file);
            }
        }
    }
}

bool BrowserControl::IsContainsFile(GalleryData* pData, const QString& file) {
    foreach(GalleryItemData* pItem, pData->getItems()) {
        if(!file.compare(pItem->getFileName(), Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

void BrowserControl::ParseUrl(const QString& url, QString& path, QString& ext) {
    path = "";
    ext = ".";

    if(!url.isEmpty()) {
        QStringList list = url.split("/");
        QStringList listExt = list[list.count() - 1].split(".");
        if(listExt.count() > 1) {
            ext = list[list.count() - 1].right(list[list.count() - 1].length() - listExt[0].length());
        }
        path = url.left(url.length() - list[list.count() - 1].length());
    }
}

void BrowserControl::CreateMenuAndActions() {
    pTitle->ToolBar()->setVisible(true);
    pmMenu = new QMenu(this);

    paAdd = new QAction("&Add gallery", this);
    paAdd->setStatusTip("Add gallery");
    paAdd->setIcon(QIcon(":/res/resources/add.png"));
    //pTitle->ToolBar()->addAction(paAdd);
    pmMenu->addAction(paAdd);
    connect(paAdd, SIGNAL(triggered()), this, SLOT(AddGalleryEvent()));

    paAddItem = new QAction("&Add to gallery", this);
    paAddItem->setStatusTip("Add to gallery items");
    paAddItem->setIcon(QIcon(":/res/resources/add.png"));
    //pTitle->ToolBar()->addAction(paAddItem);
    pmMenu->addAction(paAddItem);
    connect(paAddItem, SIGNAL(triggered()), this, SLOT(AddItemsEvent()));

    paReloadItem = new QAction("&Reload item", this);
    paReloadItem->setStatusTip("Reload active item");
    paReloadItem->setIcon(QIcon(":/res/resources/add.png"));
    //pTitle->ToolBar()->addAction(paReloadItem);
    pmMenu->addAction(paReloadItem);
    connect(paReloadItem, SIGNAL(triggered()), this, SLOT(RelodItemEvent()));
}

void BrowserControl::UpdateButtons() {
    if(pGallery) {
        bool isValid = false;
        bool isFind = false;

        QString urlPath;
        QString urlExtension;
        ParseUrl(pWeb->TopHref(), urlPath, urlExtension);
        if(Application::getInstance()->getExtensionByName(urlExtension) != NULL) {
            isValid = true;
            const QUrl& url = pWeb->url();
            QString topUrl = url.resolved(QUrl(pWeb->TopHref())).toString();
            foreach(GalleryItemData* pItem, pGallery->getItems()) {
                if(pItem->getUrl() == topUrl) {
                    isFind = true;
                    break;
                }
            }
        }

        paAdd->setEnabled(false);
        if(isValid) {
            if(!isFind) {
                paAddItem->setEnabled(true);
                paReloadItem->setEnabled(false);
            } else {
                paAddItem->setEnabled(false);
                paReloadItem->setEnabled(true);
            }
        } else {
            paAddItem->setEnabled(false);
            paReloadItem->setEnabled(false);
        }
    } else {
        paAdd->setEnabled(true);
        paAddItem->setEnabled(false);
        paReloadItem->setEnabled(false);
    }
}
