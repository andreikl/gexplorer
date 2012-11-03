#include "data/customgalleryitemdata.h"

#include "data/customgallerydata.h"

CustomGalleryData::CustomGalleryData()
{
}

CustomGalleryData::~CustomGalleryData()
{
    foreach(CustomGalleryItemData* item, items) {
        delete item;
    }
}

const QList<CustomGalleryItemData*>& CustomGalleryData::getItems() const
{
    return items;
}

const QList<KeyData*>& CustomGalleryData::getKeys() const
{
    return keys;
}

const QString& CustomGalleryData::getName() const
{
    return name;
}

int CustomGalleryData::getId() const
{
    return id;
}

void CustomGalleryData::setName(const QString& value)
{
    name = value;
}

void CustomGalleryData::setId(int value)
{
    id = value;
}

CustomGalleryData* CustomGalleryData::clone() const
{
    CustomGalleryData* gallery = new CustomGalleryData();
    gallery->id = id;
    gallery->name = name;
    foreach(const CustomGalleryItemData* item, items) {
        item->clone(*gallery);
    }
    return gallery;
}
