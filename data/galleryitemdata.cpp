#include "data/gallerydata.h"

#include "data/galleryitemdata.h"

GalleryItemData::GalleryItemData(GalleryData* gallery, const ExtensionData& extension): status(StatusNew), reference(NULL), referenceAngle(0)
{
    width = 0;
    height = 0;

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

GalleryData* GalleryItemData::getGallery() const
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

GalleryItemData* GalleryItemData::getReference() const
{
    return reference;
}

int GalleryItemData::getReferenceAngle() const
{
    return referenceAngle;
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

void GalleryItemData::setReference(GalleryItemData* reference)
{
    this->reference = reference;
}

void GalleryItemData::setReferenceAngle(int angle)
{
    referenceAngle = angle;
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
