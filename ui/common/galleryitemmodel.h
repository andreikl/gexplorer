#ifndef GALLERYITEMMODEL_H
#define GALLERYITEMMODEL_H

#include <QtCore/QMetaType>

#include <QtGui/QPixmap>

#include "config.h"

class CustomGalleryItemData;
class GalleryItemData;

class GalleryItemModel
{
public:
    GalleryItemModel(CustomGalleryItemData& item, int size);
    GalleryItemModel(GalleryItemData& item, int size);
    virtual ~GalleryItemModel();

private:
    void init(int size);

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

Q_DECLARE_METATYPE(GalleryItemModel*)

#endif // GALLERYITEMMODEL_H
