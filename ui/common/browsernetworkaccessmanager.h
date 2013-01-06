#ifndef BROWSERNETWORKACCESSMANAGER_H
#define BROWSERNETWORKACCESSMANAGER_H

#include <QtNetwork/QNetworkAccessManager>

class BrowserNetworkAccessManager: public QNetworkAccessManager
{
    Q_OBJECT

public:
    struct StreamData {
        QString url;
        QString ext;
    };

public:
    BrowserNetworkAccessManager(QObject* parent = NULL);

public:
    const QList<StreamData>& getStreams();
    void clear();

private slots:
    void dataEvent();

protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);

private:
    bool checkExist(const QString& url);

private:
    QList<StreamData> data;

};

#endif // BROWSERNETWORKACCESSMANAGER_H
