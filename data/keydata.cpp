#include "data/customgallerydata.h"

#include "data/keydata.h"

KeyData::KeyData()
{
}

KeyData::~KeyData()
{
}

const QList<CustomGalleryData*>& KeyData::getGalleries() const
{
    return galleries;
}

const QString& KeyData::getName() const
{
    return name;
}

int KeyData::getId() const
{
    return id;
}

void KeyData::setName(const QString& value)
{
    name = value;
}

void KeyData::setId(int value)
{
    id = value;
}

bool KeyData::isContainGallery(int id) const
{
    foreach(CustomGalleryData* gallery, galleries) {
        if(gallery->getId() == id) {
            return true;
        }
    }
    return false;
}

KeyData* KeyData::clone() const
{
    KeyData* key = new KeyData();
    key->id = id;
    key->name = name;
    key->galleries = galleries;
    return key;
}
