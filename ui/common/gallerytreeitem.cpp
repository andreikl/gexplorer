#include "data/customgallerydata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "ui/common/gallerytreeitem.h"

GalleryTreeItem::GalleryTreeItem(BaseData* data, const QString& name, GalleryTreeItem* parent)
{
    this->name = name;
    this->data = data;
    this->parent = parent;
}

GalleryTreeItem::~GalleryTreeItem()
{
    qDeleteAll(childs);
}

CustomGalleryData* GalleryTreeItem::getCustomGallery()
{
    return dynamic_cast<CustomGalleryData*>(data);
}

GalleryData* GalleryTreeItem::getGallery()
{
    return dynamic_cast<GalleryData*>(data);
}

KeyData* GalleryTreeItem::getKey()
{
    return dynamic_cast<KeyData*>(data);
}

GalleryTreeItem* GalleryTreeItem::getParent()
{
    return parent;
}

const QString& GalleryTreeItem::getName()
{
    return name;
}

BaseData* GalleryTreeItem::getData()
{
    return data;
}

void GalleryTreeItem::setParent(GalleryTreeItem* parent)
{
    this->parent = parent;
}

void GalleryTreeItem::setName(const QString& name)
{
    this->name = name;
}

void GalleryTreeItem::setData(BaseData* data)
{
    this->data = data;
}

QList<GalleryTreeItem*>& GalleryTreeItem::getChilds()
{
    return childs;
}
