#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QList>

class GalleryItemData;
class DownloadHandler;
class ExtensionData;

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class Application: public QObject
{
    Q_OBJECT

public:
    static Application* createInstance(const QString& path);
    static Application* getInstance();

private:
    Application();

public:
    virtual ~Application();

public:
    ExtensionData* getExtensionByName(const QString& name) const;
    const QQueue<DownloadHandler*>& getDownloaders() const;
    const QList<ExtensionData*>& getExtensions() const;
    ExtensionData* getExtensionById(int id) const;

public:
    void addToDownload(GalleryItemData& value);

signals:
    void onDownloadEnqueue(DownloadHandler* pValue);
    void onDownloadDequeue(DownloadHandler* pValue);

private slots:
    void finishEvent(DownloadHandler* pSender, bool res);

private:
    QList<ExtensionData*> extensions;
    QQueue<DownloadHandler*> queue;
    QNetworkAccessManager* network;

    int activeDownload;

private:
    static Application* app;
};

#endif // APPLICATION_H
