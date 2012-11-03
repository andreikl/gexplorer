#ifndef DOWNLOAD_HANDLER_H
#define DOWNLOAD_HANDLER_H

#include <QtCore/QMetaType>

QT_BEGIN_NAMESPACE
class QHttpResponseHeader;
class QHttp;
class QFile;
QT_END_NAMESPACE

class GalleryItemData;
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
    bool isDownload() const;
    bool load();

signals:
    void onFinish(DownloadHandler* sender, bool res);
    void onData(uint percent);

private slots:
    void dataEvent(const QHttpResponseHeader& resp);
    void finishEvent(int id, bool isError);

private:
    void cleanContent();

private:
    GalleryItemData* item;
    uint percent;
    uint loaded;
    int id;

private:
    QFile* file;
    QHttp* http;

};

Q_DECLARE_METATYPE(DownloadHandler*)

#endif // DOWNLOAD_HANDLER_H
