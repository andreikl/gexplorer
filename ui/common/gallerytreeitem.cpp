#include "data/customgallerydata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "ui/common/gallerytreeitem.h"

GalleryTreeItem::GalleryTreeItem(BaseData* pData, const QString& name, GalleryTreeItem* pParent) {
    this->name = name;
    this->pData = pData;
    this->pParent = pParent;
}

GalleryTreeItem::~GalleryTreeItem() {
    qDeleteAll(childs);
}

GalleryData* GalleryTreeItem::Gallery() {
    return dynamic_cast<GalleryData*>(pData);
}

CustomGalleryData* GalleryTreeItem::CustomGallery() {
    return dynamic_cast<CustomGalleryData*>(pData);
}

KeyData* GalleryTreeItem::Key() {
    return dynamic_cast<KeyData*>(pData);
}

BaseData* GalleryTreeItem::Data() {
    return pData;
}

void GalleryTreeItem::Data(BaseData* pData) {
    this->pData = pData;
}

const QString& GalleryTreeItem::Name() {
    return name;
}

void GalleryTreeItem::Name(const QString& name) {
    this->name = name;
}

GalleryTreeItem* GalleryTreeItem::Parent() {
    return pParent;
}

void GalleryTreeItem::Parent(GalleryTreeItem* pParent) {
    this->pParent = pParent;
}

QList<GalleryTreeItem*>& GalleryTreeItem::Childs() {
    return childs;
}
