#include <QtGui/QPainter>

#include <QtWidgets/QStyledItemDelegate>

#include "application.h"
#include "config.h"

#include "data/galleryitemdata.h"

#include "handlers/downloadhandler.h"

#include "ui/controls/titlecontrol.h"

#include "ui/windows/downloadswindow.h"
#include "ui_downloadswindow.h"

class DownloadsWindowDelegate: public QStyledItemDelegate
{
public:
    DownloadsWindowDelegate(QWidget* parent): QStyledItemDelegate(parent)
    {
    }

    virtual ~DownloadsWindowDelegate()
    {
    }

public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyledItemDelegate::paint(painter, option, index);

        if(index.data(Qt::UserRole).canConvert<DownloadHandler*>()) {
            DownloadHandler* item = qvariant_cast<DownloadHandler*>(index.data(Qt::UserRole));
            if(item != NULL) {
                QRect itemRect(option.rect);
                itemRect.setWidth(itemRect.width() - 32);

                QTextOption options(Qt::AlignRight);
                painter->setPen(option.palette.foreground().color());
                painter->drawText(itemRect, QString("(%1 %)").arg(item->getPercent()), options);
            }
        }
    }
};

DownloadsWindow::DownloadsWindow(QWidget* parent): QDockWidget(parent), ui(new Ui::DownloadsWindow)
{
    ui->setupUi(this);

    TitleControl* title =  new TitleControl(this);
    connect(title->getCloseAction(), SIGNAL(triggered()), SLOT(exitEvent()));
    setTitleBarWidget(title);

    ui->lwUrls->setItemDelegate(new DownloadsWindowDelegate(ui->lwUrls));

    connect(Application::getInstance(), SIGNAL(onDownloadDequeue(DownloadHandler*)), SLOT(dequeueEvent(DownloadHandler*)));
    connect(Application::getInstance(), SIGNAL(onDownloadEnqueue(DownloadHandler*)), SLOT(enqueueEvent(DownloadHandler*)));

    foreach(DownloadHandler* downloader, Application::getInstance()->getDownloaders()) {
        QListWidgetItem* item = new QListWidgetItem(downloader->getItem()->getFileName(), ui->lwUrls);
        item->setData(Qt::UserRole, qVariantFromValue(downloader));

        connect(downloader, SIGNAL(onData(uint)), this, SLOT(dataEvent(uint)));
    }
}

DownloadsWindow::~DownloadsWindow()
{
    delete ui;
}

void DownloadsWindow::enqueueEvent(DownloadHandler* downloader)
{
    QListWidgetItem* item = new QListWidgetItem(downloader->getItem()->getFileName(), ui->lwUrls);
    item->setData(Qt::UserRole, qVariantFromValue(downloader));

    connect(downloader, SIGNAL(onData(uint)), this, SLOT(dataEvent(uint)));
}

void DownloadsWindow::dequeueEvent(DownloadHandler* downloader)
{
    disconnect(downloader, SIGNAL(onData(uint)), this, SLOT(dataEvent(uint)));

    for(int i = 0; i < ui->lwUrls->count(); i++) {
        QListWidgetItem* item = ui->lwUrls->item(i);

        DownloadHandler* p = qvariant_cast<DownloadHandler*>(item->data(Qt::UserRole));
        if(p != NULL && p == downloader) {
            delete item;
            break;
        }
    }
}

void DownloadsWindow::dataEvent(uint /*percent*/)
{
    ui->lwUrls->viewport()->update();
}

void DownloadsWindow::exitEvent()
{
    Config::getInstance()->setIsDownloadsWindow(false);
    close();
}
