#ifndef GALLERYITEMACTION_H
#define GALLERYITEMACTION_H

#include <QtWidgets/QAction>

class CustomGalleryItemData;
class CustomGalleryData;
class GalleryItemData;
class KeyData;

class GalleryItemAction: public QAction
{
    Q_OBJECT

private:
    GalleryItemAction(QObject* parent);

public:
    GalleryItemAction(QObject* parent, KeyData* key, CustomGalleryData* gallery);
    GalleryItemAction(QObject* parent, CustomGalleryItemData* item);
    GalleryItemAction(QObject* parent, GalleryItemData* item);
    virtual ~GalleryItemAction();

public:
    CustomGalleryItemData* getCustomGalleryItem();
    GalleryItemData* getGalleryItem();

    CustomGalleryData* getCustomGallery();
    KeyData* getKey();

signals:
    void onActivateKey(KeyData* key, CustomGalleryData* gallery);
    void onActivateCustomGallery(CustomGalleryItemData* gallery);
    void onActivateGallery(GalleryItemData* gallery);

private slots:
    void triggerEvent();

private:
    CustomGalleryItemData* customGalleryItem;
    GalleryItemData* galleryItem;

    CustomGalleryData* customGallery;
    KeyData* key;
};

#endif // GALLERYITEMACTION_H
