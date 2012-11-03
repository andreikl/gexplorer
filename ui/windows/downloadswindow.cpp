#include <QtGui/QPainter>

#include "config.h"

#include "data/galleryitemdata.h"

#include "handlers/downloadhandler.h"

#include "ui/controls/titlecontrol.h"

#include "ui/windows/downloadswindow.h"

#include "ui_downloadswindow.h"

DownloadsWindowDelegate::DownloadsWindowDelegate(QWidget* pParent): QStyledItemDelegate(pParent) {
}

DownloadsWindowDelegate::~DownloadsWindowDelegate() {
}

void DownloadsWindowDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(pPainter, option, index);

    if(qVariantCanConvert<DownloadHandler*>(index.data(Qt::UserRole))) {
        DownloadHandler* pItem = qVariantValue<DownloadHandler*>(index.data(Qt::UserRole));
        if(pItem != NULL) {
            QRect itemRect(option.rect);
            itemRect.setWidth(itemRect.width() - 32);

            QTextOption options(Qt::AlignRight);
            pPainter->setPen(option.palette.foreground().color());
            pPainter->drawText(itemRect, QString("(%1 %)").arg(pItem->getPercent()), options);
        }
    }
}

DownloadsWindow::DownloadsWindow(QWidget* pParent): QDockWidget(pParent), pUi(new Ui::DownloadsWindow) {
    pUi->setupUi(this);

    TitleControl* pTitle =  new TitleControl(this);
    connect(pTitle->CloseAction(), SIGNAL(triggered()), SLOT(ExitEvent()));
    setTitleBarWidget(pTitle);

    pUi->plwUrls->setItemDelegate(new DownloadsWindowDelegate(pUi->plwUrls));

    connect(Application::getInstance(), SIGNAL(onDownloadDequeue(DownloadHandler*)), SLOT(dequeueEvent(DownloadHandler*)));
    connect(Application::getInstance(), SIGNAL(onDownloadEnqueue(DownloadHandler*)), SLOT(enqueueEvent(DownloadHandler*)));

    foreach(DownloadHandler* pDownloader, Application::getInstance()->getDownloaders()) {
        QListWidgetItem* pItem = new QListWidgetItem(pDownloader->getItem()->getFileName(), pUi->plwUrls);
        pItem->setData(Qt::UserRole, qVariantFromValue(pDownloader));

        connect(pDownloader, SIGNAL(onData(uint)), this, SLOT(dataEvent(uint)));
    }
}

DownloadsWindow::~DownloadsWindow() {
    delete pUi;
}

void DownloadsWindow::enqueueEvent(DownloadHandler* pDownloader) {
    QListWidgetItem* pItem = new QListWidgetItem(pDownloader->getItem()->getFileName(), pUi->plwUrls);
    pItem->setData(Qt::UserRole, qVariantFromValue(pDownloader));

    connect(pDownloader, SIGNAL(onData(uint)), this, SLOT(dataEvent(uint)));
}

void DownloadsWindow::dequeueEvent(DownloadHandler* pDownloader) {
    disconnect(pDownloader, SIGNAL(onData(uint)), this, SLOT(dataEvent(uint)));

    for(int i = 0; i < pUi->plwUrls->count(); i++) {
        QListWidgetItem* pItem = pUi->plwUrls->item(i);

        DownloadHandler* p = qVariantValue<DownloadHandler*>(pItem->data(Qt::UserRole));
        if(p != NULL && p == pDownloader) {
            delete pItem;
            break;
        }
    }
}

void DownloadsWindow::dataEvent(uint /*percent*/)
{
    pUi->plwUrls->viewport()->update();
}

void DownloadsWindow::ExitEvent() {
    Config::Self()->IsDownloadsWindow(false);
    close();
}
