#ifndef GALLERYLISTITEM_H
#define GALLERYLISTITEM_H

#include <QtCore/QMetaType>

#include <QtGui/QPixmap>

#include "config.h"

class CustomGalleryItemData;
class GalleryItemData;

class GalleryListItem {
public:
    GalleryListItem(GalleryItemData& item, int size);
    GalleryListItem(CustomGalleryItemData& item, int size);
    virtual ~GalleryListItem();

public:
    Config::ItemTypeEnum Type();
    void* Item();

public:
    const QPixmap& Pixmap();
    const QString& Name();

private:
    void LoadPixmap(GalleryItemData& item, int size);
    void LoadPixmap(CustomGalleryItemData& item, int size);

private:
    Config::ItemTypeEnum type;
    void* pItem;

private:
    QPixmap pixmap;
};

Q_DECLARE_METATYPE(GalleryListItem*)

#endif // GALLERYLISTITEM_H
