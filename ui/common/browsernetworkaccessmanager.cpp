#include <QtCore/QDebug>

#include <QtNetwork/QNetworkReply>

#include "ui/common/browsernetworkaccessmanager.h"

BrowserNetworkAccessManager::BrowserNetworkAccessManager(QObject* parent): QNetworkAccessManager(parent)
{
}

const QList<BrowserNetworkAccessManager::StreamData>& BrowserNetworkAccessManager::getStreams() {
    return data;
}

void BrowserNetworkAccessManager::clear() {
    data.clear();
}

void BrowserNetworkAccessManager::dataEvent() {
    QNetworkReply* reply = reinterpret_cast<QNetworkReply*>(QObject::sender());
    long long length = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    const QString& type = reply->header(QNetworkRequest::ContentTypeHeader).toString();

    /*qDebug() << reply->request().url();
    foreach(const QNetworkReply::RawHeaderPair& h, reply->rawHeaderPairs()) {
        qDebug() << h.first << ": " << h.second;
    }*/

    QString url = reply->request().url().toString();
    if(length > 200000) {
        /*qDebug() << reply->request().url();
        foreach(const QNetworkReply::RawHeaderPair& h, reply->rawHeaderPairs()) {
            qDebug() << h.first << ": " << h.second;
        }*/

        if(type.compare("video/mpeg", Qt::CaseInsensitive) == 0 || type.compare("video/mp4", Qt::CaseInsensitive) == 0
                || type.compare("video/x-ms-wmv", Qt::CaseInsensitive) == 0 || type.compare("video/x-flv", Qt::CaseInsensitive) == 0) {
            if(!checkExist(url)) {
                StreamData data;
                data.url = url;
                if(type.compare("video/mpeg", Qt::CaseInsensitive) == 0 || type.compare("video/mp4", Qt::CaseInsensitive) == 0) {
                    data.ext = ".mpg";
                } else if(type.compare("video/x-ms-wmv", Qt::CaseInsensitive) == 0) {
                    data.ext = ".wmv";
                } else if(type.compare("video/x-flv", Qt::CaseInsensitive) == 0) {
                    data.ext = ".flv";

                }
                this->data.append(data);
            }
        }
    } else if(url.endsWith(".flv", Qt::CaseInsensitive)) {
        if(!checkExist(url)) {
            StreamData data;
            data.url = url;
            data.ext = ".flv";
            this->data.append(data);
        }
    }
}

QNetworkReply* BrowserNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) {
    /*qDebug() << request.rawHeaderList();
    qDebug() << request.url();*/
    QNetworkReply* reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    connect(reply, SIGNAL(readyRead()), SLOT(dataEvent()));
    return reply;
}

bool BrowserNetworkAccessManager::checkExist(const QString& url) {
    foreach(const StreamData& data, this->data) {
        if(data.url.compare(url, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}
