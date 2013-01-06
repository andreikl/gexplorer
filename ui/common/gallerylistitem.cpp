#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"

#include "helpers/gstreamerhelper.h"
#include "helpers/commonhelper.h"

#include "ui/common/gallerylistitem.h"

GalleryListItem::GalleryListItem(int size)
{
    isLoad = false;
    this->size = size;

    pixmap = QPixmap(size, size);
    pixmap.fill(Qt::white);
}

GalleryListItem::GalleryListItem(CustomGalleryItemData& item, int size): GalleryListItem(size)
{
    this->item = &item;

    type = Config::ETypeCustomGalleryItem;
}

GalleryListItem::GalleryListItem(GalleryItemData& item, int size): GalleryListItem(size)
{
    this->item = &item;

    type = Config::ETypeGalleryItem;
}

GalleryListItem::~GalleryListItem()
{
}

Config::ItemTypeEnum GalleryListItem::getType()
{
    return type;
}

int GalleryListItem::getSize()
{
    return size;
}

void* GalleryListItem::getItem()
{
    return item;
}

const QPixmap& GalleryListItem::getPixmap()
{
    return pixmap;
}

const QString& GalleryListItem::getName()
{
    if(type == Config::ETypeGalleryItem) {
        return reinterpret_cast<GalleryItemData*>(item)->getFileName();
    } else {
        return reinterpret_cast<CustomGalleryItemData*>(item)->getName();
    }
}

void GalleryListItem::loadPixmap(int size)
{
    if(type == Config::ETypeGalleryItem) {
        return loadPixmap(*reinterpret_cast<GalleryItemData*>(item), size);
    } else {
        return loadPixmap(*reinterpret_cast<CustomGalleryItemData*>(item), size);
    }
}

void GalleryListItem::loadPixmap(CustomGalleryItemData& item, int size)
{
    isLoad = true;
    const QString& path = CommonHelper::getPath(item);
    if(item.getItem().getExtension().getType() == ExtensionData::ExtensionVideo) {
        const QPixmap& source = GStreamerHelper::getInstance()->snapshot(path);
        if(!source.isNull()) {
            item.width = source.width();
            item.height = source.height();
            if(source.width() > source.height()) {
                pixmap = source.scaledToWidth(size);
            } else {
                pixmap = source.scaledToHeight(size);
            }
        }
    } else if(item.getItem().getExtension().getType() == ExtensionData::ExtensionImage) {
        const QPixmap& source(path);
        if(!source.isNull()) {
            if(source.width() > source.height()) {
                pixmap = source.scaledToWidth(size);
            } else {
                pixmap = source.scaledToHeight(size);
            }
        }
    }
    int angle = CommonHelper::getAngle(item);
    if(angle != 0) {
        QMatrix rm;
        rm.rotate(angle);

        pixmap = pixmap.transformed(rm);
    }
}

void GalleryListItem::loadPixmap(GalleryItemData& item, int size)
{
    isLoad = true;
    const QString& path = CommonHelper::getPath(item);
    if(item.getExtension().getType() == ExtensionData::ExtensionVideo) {
        const QPixmap& source = GStreamerHelper::getInstance()->snapshot(path);
        if(!source.isNull()) {
            item.width = source.width();
            item.height = source.height();
            if(source.width() > source.height()) {
                pixmap = source.scaledToWidth(size);
            } else {
                pixmap = source.scaledToHeight(size);
            }
        }
    } else if(item.getExtension().getType() == ExtensionData::ExtensionImage) {
        const QPixmap& source(path);
        if(!source.isNull()) {
            if(source.width() > source.height()) {
                pixmap = source.scaledToWidth(size);
            } else {
                pixmap = source.scaledToHeight(size);
            }
        }
    }
    int angle = CommonHelper::getAngle(item);
    if(angle != 0) {
        QMatrix rm;
        rm.rotate(angle);

        pixmap = pixmap.transformed(rm);
    }
}
