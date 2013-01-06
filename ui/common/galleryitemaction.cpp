#include "ui/common/galleryitemaction.h"

GalleryItemAction::GalleryItemAction(QObject* parent): QAction(parent)
{
    customGalleryItem = NULL;
    galleryItem = NULL;

    customGallery = NULL;
    key = NULL;
}

GalleryItemAction::GalleryItemAction(QObject* parent, KeyData* key, CustomGalleryData* gallery): GalleryItemAction(parent)
{
    this->key = key;
    this->customGallery = gallery;
    connect(this, SIGNAL(triggered()), SLOT(triggerEvent()));
}

GalleryItemAction::GalleryItemAction(QObject* parent, CustomGalleryItemData* item): GalleryItemAction(parent)
{
    this->customGalleryItem = item;
    connect(this, SIGNAL(triggered()), SLOT(triggerEvent()));
}

GalleryItemAction::GalleryItemAction(QObject* parent, GalleryItemData* item): GalleryItemAction(parent)
{
    this->galleryItem = item;
    connect(this, SIGNAL(triggered()), SLOT(triggerEvent()));
}

GalleryItemAction::~GalleryItemAction()
{
    customGalleryItem = NULL;
    galleryItem = NULL;

    customGallery = NULL;
    key = NULL;
}

CustomGalleryItemData* GalleryItemAction::getCustomGalleryItem()
{
    return customGalleryItem;
}

GalleryItemData* GalleryItemAction::getGalleryItem()
{
    return galleryItem;
}

CustomGalleryData* GalleryItemAction::getCustomGallery()
{
    return customGallery;
}

KeyData* GalleryItemAction::getKey()
{
    return key;
}

void GalleryItemAction::triggerEvent()
{
    if(customGalleryItem) {
        emit onActivateCustomGallery(customGalleryItem);
    } else if(galleryItem) {
        emit onActivateGallery(galleryItem);
    } else if(key && customGallery) {
        emit onActivateKey(key, customGallery);
    }
}
