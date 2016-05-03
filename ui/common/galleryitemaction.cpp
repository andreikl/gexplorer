#include <QtWidgets/QAction>

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "ui/common/galleryitemaction.h"

GalleryItemAction::GalleryItemAction(QObject* parent, KeyData* key, CustomGalleryData* gallery): QAction(parent)
{
    init();

    this->key = key;
    this->customGallery = gallery;
    connect(this, SIGNAL(triggered()), SLOT(triggerEvent()));
}

GalleryItemAction::GalleryItemAction(QObject* parent, CustomGalleryItemData* item): QAction(parent)
{
    init();

    this->customGalleryItem = item;
    connect(this, SIGNAL(triggered()), SLOT(triggerEvent()));
}

GalleryItemAction::GalleryItemAction(QObject* parent, GalleryItemData* item): QAction(parent)
{
    init();

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

void GalleryItemAction::init()
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
