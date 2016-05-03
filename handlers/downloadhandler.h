#ifndef DOWNLOAD_HANDLER_H
#define DOWNLOAD_HANDLER_H

#include <QtCore/QMetaType>

#include <QtNetwork/QNetworkAccessManager>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
class QFile;
QT_END_NAMESPACE

class GalleryItemData;
struct NamePoolData;
class GalleryData;

class DownloadHandler: public QObject
{
    Q_OBJECT

public:
    DownloadHandler(const GalleryItemData& item);
    virtual ~DownloadHandler();

public:
    const GalleryItemData* getItem() const;
    uint getPercent() const;

public:
    bool isError() const;
    bool isDownload() const;
    bool load(QNetworkAccessManager* network);

signals:
    void onFinish(DownloadHandler* sender, bool res);
    void onData(uint percent);

private slots:
    void finishEvent();
    void dataEvent();

private:
    void cleanContent();

private:
    GalleryItemData* item;
    GalleryData* gallery;
    NamePoolData* data;
    uint percent;
    uint loaded;

private:
    QNetworkAccessManager* network;
    QNetworkReply* reply;
    QFile* file;
};

Q_DECLARE_METATYPE(DownloadHandler*)

#endif // DOWNLOAD_HANDLER_H
