#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/namespoolhandler.h"

#include "handlers/downloadhandler.h"

DownloadHandler::DownloadHandler(const GalleryItemData& item): data(NULL), reply(NULL), file(NULL)
{
    this->item = item.clone(NULL);
    gallery = item.getGallery();

    cleanContent();
}

DownloadHandler::~DownloadHandler()
{
    cleanContent();

    if(reply) {
        delete reply;
        reply = NULL;
    }

    if(file) {
        delete file;
        file = NULL;
    }

    if(item != NULL) {
        delete item;
        item = NULL;
    }
    gallery = NULL;
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
    return reply && reply->isRunning();
}

bool DownloadHandler::load(QNetworkAccessManager* network)
{
    bool res = false;

    this->network = network;

    data = NamesPoolHandler::getInstance()->GetEmptyData();
    data->isBusy = true;

    file = new QFile(QDir::currentPath() + QDir::separator() + data->name);
    if(file->exists()) {
        file->remove();
    }
    file->open(QIODevice::WriteOnly);

    QNetworkRequest req(QUrl(item->getUrl()));
    req.setRawHeader(QString("Referer").toLatin1(), gallery->getSource().toLatin1());

    reply = network->get(req);
    connect(reply, SIGNAL(readyRead()), SLOT(dataEvent()));
    connect(reply, SIGNAL(finished()), SLOT(finishEvent()));

    if(file && file->isOpen() && reply->error() == QNetworkReply::NoError) {
        res = true;
    } else {
        cleanContent();
    }

    return res;
}

void DownloadHandler::finishEvent()
{
    bool res = false;

    if(reply->error() == QNetworkReply::NoError) {
        //qDebug() << reply->rawHeaderPairs();
        //qDebug() << reply->url();

        QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        if(!redirectUrl.isEmpty() && redirectUrl != reply->url()) {
            delete reply;

            QNetworkRequest req(redirectUrl);
            req.setRawHeader(QString("Referer").toLatin1(), gallery->getSource().toLatin1());

            reply = network->get(req);
            connect(reply, SIGNAL(readyRead()), SLOT(dataEvent()));
            connect(reply, SIGNAL(finished()), SLOT(finishEvent()));

            return;
        }

        if(file->isOpen()) {
            file->close();
        }

        QDir dir(QDir::currentPath() + QDir::separator() + gallery->getPath());
        if(!dir.exists()) {
            dir.mkpath(dir.path());
        }

        QString dest = dir.path() + QDir::separator() + item->getFileName();
        if(QFile::exists(dest)) {
            QFile::remove(dest);
        }
        res = dir.rename(QDir::currentPath() + QDir::separator() + data->name, dest);

        if(data->isBusy) {
            data->isBusy = false;
        }

    } else {
        qDebug() << "download finish" << reply->errorString();

        /*if (loaded > 0) {
            QNetworkRequest req(QUrl(item->getUrl()));
            req.setRawHeader(QString("Referer").toLatin1(), gallery->getSource().toLatin1());
            const QString& str = QString("bytes=") + QString(loaded);
            req.setRawHeader(QString("Range").toLatin1(), str.toLatin1());

            reply = network->get(req);
            connect(reply, SIGNAL(readyRead()), SLOT(dataEvent()));
            connect(reply, SIGNAL(finished()), SLOT(finishEvent()));

            if(file && file->isOpen() && reply->error() == QNetworkReply::NoError) {
                qDebug() << "continue downloading from: " << str;
            } else {
                cleanContent();
                emit onFinish(this, res);
            }
        } else {
            cleanContent();
            emit onFinish(this, res);
        }*/
    }

    cleanContent();
    emit onFinish(this, res);

    //delete reply;
    //reply = NULL;
}

void DownloadHandler::dataEvent()
{
    if(reply->error() == QNetworkReply::NoError) {
        qint64 wlen = file->write(reply->readAll());
        if(wlen != -1) {
            loaded += (uint)wlen;
        }

        percent = (uint)(loaded / (double)reply->header(QNetworkRequest::ContentLengthHeader).toLongLong() * 100);
        emit onData(percent);
    } else {
        qDebug() << "download data" << reply->errorString();
    }
}

void DownloadHandler::cleanContent()
{
    percent = 0;
    loaded = 0;

    if(file && file->isOpen()) {
        file->close();
    }

    if(file && file->exists()) {
        file->remove();
    }

    if(data && data->isBusy) {
        data->isBusy = false;
    }
}
