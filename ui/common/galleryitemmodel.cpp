#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"

#include "helpers/gstreamerhelper.h"
#include "helpers/commonhelper.h"

#include "ui/common/galleryitemmodel.h"

GalleryItemModel::GalleryItemModel(CustomGalleryItemData& item, int size)
{
    init(size);

    this->item = &item;

    type = Config::ETypeCustomGalleryItem;
}

GalleryItemModel::GalleryItemModel(GalleryItemData& item, int size)
{
    init(size);

    this->item = &item;

    type = Config::ETypeGalleryItem;
}

GalleryItemModel::~GalleryItemModel()
{
}

void GalleryItemModel::init(int size)
{
    isLoad = false;
    this->size = size;

    pixmap = QPixmap(size, size);
    pixmap.fill(Qt::white);
}

Config::ItemTypeEnum GalleryItemModel::getType()
{
    return type;
}

int GalleryItemModel::getSize()
{
    return size;
}

void* GalleryItemModel::getItem()
{
    return item;
}

const QPixmap& GalleryItemModel::getPixmap()
{
    return pixmap;
}

const QString& GalleryItemModel::getName()
{
    if(type == Config::ETypeGalleryItem) {
        return reinterpret_cast<GalleryItemData*>(item)->getFileName();
    } else {
        return reinterpret_cast<CustomGalleryItemData*>(item)->getName();
    }
}

void GalleryItemModel::loadPixmap(int size)
{
    if(type == Config::ETypeGalleryItem) {
        return loadPixmap(*reinterpret_cast<GalleryItemData*>(item), size);
    } else {
        return loadPixmap(*reinterpret_cast<CustomGalleryItemData*>(item), size);
    }
}

void GalleryItemModel::loadPixmap(CustomGalleryItemData& item, int size)
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

void GalleryItemModel::loadPixmap(GalleryItemData& item, int size)
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
