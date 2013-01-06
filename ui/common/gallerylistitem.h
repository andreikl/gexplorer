#ifndef GALLERYLISTITEM_H
#define GALLERYLISTITEM_H

#include <QtCore/QMetaType>

#include <QtGui/QPixmap>

#include "config.h"

class CustomGalleryItemData;
class GalleryItemData;

class GalleryListItem
{
private:
    GalleryListItem(int size);

public:
    GalleryListItem(CustomGalleryItemData& item, int size);
    GalleryListItem(GalleryItemData& item, int size);
    virtual ~GalleryListItem();

public:
    Config::ItemTypeEnum getType();
    int getSize();

    void* getItem();

    const QPixmap& getPixmap();
    const QString& getName();

    void loadPixmap(int size);
    void loadPixmap(CustomGalleryItemData& item, int size);
    void loadPixmap(GalleryItemData& item, int size);

    bool isLoad;

private:
    Config::ItemTypeEnum type;
    int size;

    void* item;

private:
    QPixmap pixmap;

};

Q_DECLARE_METATYPE(GalleryListItem*)

#endif // GALLERYLISTITEM_H
