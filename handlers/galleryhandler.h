#ifndef GALLERYHANDLER_H
#define GALLERYHANDLER_H

#include <QtCore/QObject>

class CustomGalleryItemData;
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
    void addToWebGallery(const GalleryData& value);
    void addWebGallery(const GalleryData& value);
    void addFileGallery(const QString& source);

    bool delFromGallery(GalleryItemData& value);
    bool delGallery(GalleryData& value);

    bool updGalleryItemReference(const CustomGalleryItemData& item, const CustomGalleryItemData& reference);

signals:
    void onUpdGalleryItem(GalleryItemData* value);
    void onUpdGallery(GalleryData* value);
    void onAddGallery(GalleryData* value);
    void onDelGalleryItem(GalleryItemData* value);
    void onDelGallery(GalleryData* value);

private:
    GalleryData* getGalleryByPath(const QString& path) const;
    GalleryData* getGalleryById(int id) const;

    GalleryItemData* getGalleryItemByUrl(GalleryData* gallery, const QString& url) const;
    GalleryItemData* getGalleryItemById(int galleryId, int id) const;
    GalleryItemData* getGalleryItemById(int id) const;

private:
    static GalleryHandler* handler;

private:
    QList<GalleryData*> galleries;
};

#endif // GALLERYHANDLER_H
