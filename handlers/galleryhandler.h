#ifndef GALLERYHANDLER_H
#define GALLERYHANDLER_H

#include <QtCore/QObject>

class GalleryItemData;
class Application;
class GalleryData;

class GalleryHandler: public QObject
{
    friend class CustomGalleryHandler;
    friend class Application;
    friend class DbHandler;
    Q_OBJECT

public:
    static GalleryHandler* createInstance(Application* app);
    static GalleryHandler* getInstance();

private:
    GalleryHandler();

public:
    virtual ~GalleryHandler();

public:
    const QList<GalleryData*>& getGalleries() const;

public:
    void addWebGallery(const GalleryData& value);
    void addFileGallery(const QString& source);
    bool delGallery(GalleryData& value);

signals:
    void onUpdGallery(GalleryData* value);
    void onAddGallery(GalleryData* value);
    void onDelGallery(GalleryData* value);

private:
    GalleryItemData* getGalleryItemById(int galleryId, int id) const;
    GalleryItemData* getGalleryItemById(int id) const;
    GalleryData* getGalleryById(int id) const;

private:
    static GalleryHandler* handler;

private:
    QList<GalleryData*> galleries;
};

#endif // GALLERYHANDLER_H
