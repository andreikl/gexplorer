#include "data/gallerydata.h"

#include "data/galleryitemdata.h"

GalleryItemData::GalleryItemData(GalleryData* gallery, const ExtensionData& extension): status(StatusNew)
{
    this->gallery = gallery;
    this->extension = &extension;

    if(gallery) {
        gallery->items.append(this);
    }
}

GalleryItemData::~GalleryItemData()
{
    if(gallery) {
        gallery->items.removeOne(this);
    }
}

const QList<CustomGalleryItemData*>& GalleryItemData::getCustomItems() const
{
    return customItems;
}

const ExtensionData& GalleryItemData::getExtension() const
{
    return *extension;
}

const GalleryData* GalleryItemData::getGallery() const
{
    return gallery;
}

GalleryItemData::GalleryItemStatusEnum GalleryItemData::getStatus() const
{
    return status;
}

const QString& GalleryItemData::getFileName() const
{
    return fileName;
}

const QString& GalleryItemData::getPath() const
{
    return path;
}

const QString& GalleryItemData::getUrl() const
{
    return url;
}

int GalleryItemData::getId() const
{
    return id;
}

void GalleryItemData::setStatus(GalleryItemStatusEnum value)
{
    status = value;
}

void GalleryItemData::setFileName(const QString& value)
{
    fileName = value;
}

void GalleryItemData::setPath(const QString& value)
{
    path = value;
}

void GalleryItemData::setUrl(const QString& value)
{
    url = value;
}

void GalleryItemData::setId(int value)
{
    id = value;
}

GalleryItemData* GalleryItemData::clone(GalleryData* gallery) const
{
    GalleryItemData* item = new GalleryItemData(gallery, *extension);
    item->id = id;
    item->url = url;
    item->path = path;
    item->fileName = fileName;
    item->status = status;

    return item;
}
