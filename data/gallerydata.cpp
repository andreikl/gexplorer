#include "data/galleryitemdata.h"

#include "data/gallerydata.h"

GalleryData::GalleryData(GalleryTypeEnum type)
{
    this->id = 0;
    this->type = type;
}

GalleryData::~GalleryData()
{
    foreach(GalleryItemData* item, items) {
        delete item;
    }
}

const QList<GalleryItemData*>& GalleryData::getItems() const
{
    return items;
}

GalleryData::GalleryTypeEnum GalleryData::getType() const
{
    return type;
}

const QString& GalleryData::getSource() const
{
    return source;
}

const QString& GalleryData::getPath() const
{
    return path;
}

int GalleryData::getId() const
{
    return id;
}

void GalleryData::setSource(const QString& value)
{
    source = value;
}

void GalleryData::setPath(const QString& value)
{
    path = value;
}

void GalleryData::setId(int value)
{
    id = value;
}

bool GalleryData::isAllContains() const
{
    foreach(GalleryItemData* item, items) {
        if(item->getCustomItems().count() == 0) {
            return false;
        }
    }
    return true;
}

GalleryData* GalleryData::clone(bool isItems) const
{
    GalleryData* gallery = new GalleryData(type);
    gallery->id = id;
    gallery->source = source;
    gallery->path = path;
    if(isItems) {
        foreach(GalleryItemData* item, items) {
            item->clone(gallery);
        }
    }
    return gallery;
}
