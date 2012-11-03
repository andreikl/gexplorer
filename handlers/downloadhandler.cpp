#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QUrl>

#include <QtNetwork/QHttp>

#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/downloadhandler.h"

DownloadHandler::DownloadHandler(const GalleryItemData& item): file(NULL), http(NULL)
{
    this->item = item.clone(NULL);
    cleanContent();

    QDir dir(QDir::currentPath() + QDir::separator() + item.getGallery()->getPath());
    if(!dir.exists()) {
        dir.mkpath(dir.path());
    }
    file = new QFile(dir.path() + QDir::separator() + item.getFileName());

    http = new QHttp(this);
    connect(http, SIGNAL(readyRead(QHttpResponseHeader)), SLOT(dataEvent(QHttpResponseHeader)));
    connect(http, SIGNAL(requestFinished(int, bool)), SLOT(finishEvent(int, bool)));
}

DownloadHandler::~DownloadHandler()
{
    cleanContent();

    if(http) {
        delete http;
        http = NULL;
    }

    if(file) {
        delete file;
        file = NULL;
    }

    if(item != NULL) {
        delete item;
        item = NULL;
    }
}

const GalleryItemData* DownloadHandler::getItem() const
{
    return item;
}

uint DownloadHandler::getPercent() const
{
    return percent;
}

bool DownloadHandler::isDownload() const
{
    return (id != -1)? true: false;
}

bool DownloadHandler::load()
{
    bool res = false;

    file->open(QIODevice::WriteOnly);

    QUrl u(item->getUrl());
    if(u.port() > 0) {
        http->setHost(u.host(), u.port());
    } else {
        http->setHost(u.host());
    }
    id = http->get(u.path());

    if(file && file->isOpen() && id >= 0) {
        res = true;
    } else {
        cleanContent();
    }

    return res;
}

void DownloadHandler::dataEvent(const QHttpResponseHeader& resp)
{
    if(resp.statusCode() == 200) {
        qint64 wlen = file->write(http->readAll());
        if(wlen != -1) {
            loaded += (uint)wlen;
        }

        if(resp.hasContentLength()) {
            percent = (uint)(loaded / (double)resp.contentLength() * 100);
            emit onData(percent);
        }
    }
}

void DownloadHandler::finishEvent(int id, bool isError)
{
    if(id == this->id) {
        bool res = false;

        if(!isError) {
            QHttpResponseHeader responce = http->lastResponse();
            if(responce.statusCode() == 200) {
                res = true;
            } else  if(responce.statusCode() == 302) {
                QString url = responce.value("Location");
                //qDebug() << "redirect to:" << url;

                QUrl u(url);
                if(u.port() > 0) {
                    http->setHost(u.host(), u.port());
                } else {
                    http->setHost(u.host());
                }
                this->id = http->get(u.path());
                return;
            }
        }

        cleanContent();

        emit onFinish(this, res);
    }
}

void DownloadHandler::cleanContent()
{
    percent = 0;
    loaded = 0;
    id = -1;

    if(http) {
        http->close();
    }

    if(file && file->isOpen()) {
        file->close();
    }
}
