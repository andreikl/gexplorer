#include "data/customgallerydata.h"

#include "data/customgalleryitemdata.h"

CustomGalleryItemData::CustomGalleryItemData(CustomGalleryData& gallery, GalleryItemData& data): item(data), name(""), customId(0), angle(0), id(0)
{
    width = 0;
    height = 0;

    this->gallery = &gallery;
    this->gallery->items.append(this);
}

CustomGalleryItemData::~CustomGalleryItemData()
{
    gallery->items.removeOne(this);
}

const QList<CustomGalleryItemData*>& CustomGalleryItemData::getChildren() const
{
    return children;
}

CustomGalleryData* CustomGalleryItemData::getCustomGallery() const
{
    return gallery;
}

GalleryItemData& CustomGalleryItemData::getItem() const
{
    return item;
}

const QString& CustomGalleryItemData::getName() const
{
    return name;
}

int CustomGalleryItemData::getCustomId() const
{
    return customId;
}

int CustomGalleryItemData::getAngle() const
{
    return angle;
}

int CustomGalleryItemData::getId() const
{
    return id;
}

void CustomGalleryItemData::setName(const QString& value)
{
    name = value;
}

void CustomGalleryItemData::setCustomId(int value)
{
    customId = value;
}

void CustomGalleryItemData::setAngle(int value)
{
    angle = value;
}

void CustomGalleryItemData::setId(int value)
{
    id = value;
}

CustomGalleryItemData* CustomGalleryItemData::clone(CustomGalleryData& gallery) const
{
    CustomGalleryItemData* i = new CustomGalleryItemData(gallery, item);
    i->id = id;
    i->name = name;
    i->customId = customId;
    return i;
}
