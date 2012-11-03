#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"

#include "helpers/gstreamerhelper.h"
#include "helpers/commonhelper.h"

#include "ui/common/gallerylistitem.h"

GalleryListItem::GalleryListItem(GalleryItemData& item, int size): pItem(&item) {
    type = Config::ETypeGalleryItem;

    pixmap = QPixmap(size, size);
    pixmap.fill(Qt::white);

    LoadPixmap(item, size);
}

GalleryListItem::GalleryListItem(CustomGalleryItemData& item, int size): pItem(&item) {
    type = Config::ETypeCustomGalleryItem;

    pixmap = QPixmap(size, size);
    pixmap.fill(Qt::white);

    LoadPixmap(item, size);
}

GalleryListItem::~GalleryListItem() {
}

Config::ItemTypeEnum GalleryListItem::Type() {
    return type;
}

void* GalleryListItem::Item() {
    return pItem;
}

const QPixmap& GalleryListItem::Pixmap() {
    return pixmap;
}

const QString& GalleryListItem::Name() {
    if(type == Config::ETypeGalleryItem) {
        return reinterpret_cast<GalleryItemData*>(pItem)->getFileName();
    } else {
        return reinterpret_cast<CustomGalleryItemData*>(pItem)->getName();
    }
}

void GalleryListItem::LoadPixmap(GalleryItemData& item, int size) {
    const QString& path = CommonHelper::getPath(item);
    if(item.getExtension().getType() == ExtensionData::ExtensionVideo) {
        const QPixmap& source = GStreamerHelper::getInstance()->snapshot(path);
        if(!source.isNull()) {
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
}

void GalleryListItem::LoadPixmap(CustomGalleryItemData& item, int size) {
    const QString& path = CommonHelper::getPath(item);
    if(item.getItem().getExtension().getType() == ExtensionData::ExtensionVideo) {
        const QPixmap& source = GStreamerHelper::getInstance()->snapshot(path);
        if(!source.isNull()) {
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
    if(item.getAngle() != 0) {
        QMatrix rm;
        rm.rotate(item.getAngle());

        pixmap = pixmap.transformed(rm);
    }
}
