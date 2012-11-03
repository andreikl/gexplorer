#include "application.h"

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "handlers/galleryhandler.h"
#include "handlers/keyhandler.h"
#include "handlers/dbhandler.h"

#include "handlers/customgalleryhandler.h"

CustomGalleryHandler* CustomGalleryHandler::handler = NULL;

CustomGalleryHandler* CustomGalleryHandler::createInstance(Application*)
{
    if(!handler) {
        CustomGalleryHandler* tempHandler = new CustomGalleryHandler();
        if(DbHandler::getInstance()->getCustomGalleries(tempHandler->customGalleries)) {
            foreach(CustomGalleryData* gallery, tempHandler->customGalleries) {
                foreach(CustomGalleryItemData* item, gallery->getItems()) {
                    item->item.customItems.append(item);

                    if(item->getCustomId() != 0) {
                        CustomGalleryItemData* i = tempHandler->getCustomGalleryItemById(gallery->getId(), item->getCustomId());
                        i->children.append(item);
                    }
                }
            }
            handler = tempHandler;
        } else {
            delete tempHandler;
        }
    }
    return handler;
}

CustomGalleryHandler* CustomGalleryHandler::getInstance()
{
    return handler;
}

CustomGalleryHandler::CustomGalleryHandler()
{
}

CustomGalleryHandler::~CustomGalleryHandler()
{
    handler = NULL;
    foreach(CustomGalleryData* gallery, customGalleries) {
        delete gallery;
    }
}

const QList<CustomGalleryData*>& CustomGalleryHandler::getCustomGalleries() const
{
    return customGalleries;
}

CustomGalleryItemData* CustomGalleryHandler::addToCustomGallery(CustomGalleryData& value, int galleryId, int itemId)
{
    GalleryItemData* item = GalleryHandler::getInstance()->getGalleryItemById(galleryId, itemId);
    if(item) {
        CustomGalleryItemData* i = new CustomGalleryItemData(value, *item);
        i->setName(item->getFileName());
        if(DbHandler::getInstance()->addToCustomGallery(*i)) {
            item->customItems.append(i);

            emit GalleryHandler::getInstance()->onUpdGallery(i->getItem().gallery);
            emit onAddCustomGalleryItem(i);
            return i;
        } else {
            delete i;
        }
    }
    return NULL;
}

CustomGalleryData* CustomGalleryHandler::addCustomGallery(const CustomGalleryData& value)
{
    CustomGalleryData* gallery = value.clone();
    if(DbHandler::getInstance()->addCustomGallery(*gallery)) {
        customGalleries.append(gallery);
        onAddCustomGallery(gallery);

        QList<GalleryData*> galleries;
        foreach(CustomGalleryItemData* item, gallery->getItems()) {
            item->item.customItems.append(item);
            if(galleries.indexOf(item->getItem().gallery) < 0) {
                galleries.append(item->getItem().gallery);
            }
        }

        foreach(GalleryData* item, galleries) {
            emit GalleryHandler::getInstance()->onUpdGallery(item);
        }
        return gallery;
    } else {
        delete gallery;
    }
    return NULL;
}

CustomGalleryData* CustomGalleryHandler::addCustomGallery(int galleryId)
{
    CustomGalleryData* res = NULL;

    GalleryData* gallery = GalleryHandler::getInstance()->getGalleryById(galleryId);
    if(gallery) {
        CustomGalleryData* customGallery = new CustomGalleryData();
        customGallery->setName(gallery->getSource());

        int n = 1;
        foreach(GalleryItemData* item, gallery->getItems()) {
            CustomGalleryItemData* customItem = new CustomGalleryItemData(*customGallery, *item);
            customItem->setName(QString("%1").arg(n++, 3, 10, QChar('0')));
        }

        res = addCustomGallery(*customGallery);

        delete customGallery;
    }
    return res;
}

bool CustomGalleryHandler::delFromCustomGallery(CustomGalleryItemData& value)
{
    if(DbHandler::getInstance()->delFromCustomGallery(value)) {
        foreach(CustomGalleryItemData* item, value.getChildren()) {
            item->setCustomId(0);
            emit onUpdCustomGalleryItem(item);
        }

        emit onDelCustomGalleryItem(&value);

        value.item.customItems.removeOne(&value);
        emit GalleryHandler::getInstance()->onUpdGallery(value.getItem().gallery);

        delete &value;
        return true;
    }
    return false;
}

bool CustomGalleryHandler::delCustomGallery(CustomGalleryData& value)
{
    if(DbHandler::getInstance()->delCustomGallery(value)) {
        QList<KeyData*> keys;
        foreach(KeyData* key, value.getKeys()) {
            key->galleries.removeOne(&value);
            if(keys.indexOf(key) < 0) {
                keys.append(key);
            }
        }
        foreach(KeyData* item, keys) {
            emit KeyHandler::getInstance()->onUpdKey(item);
        }
        QList<GalleryData*> galleries;
        foreach(CustomGalleryItemData* item, value.getItems()) {
            item->item.customItems.removeOne(item);
            if(galleries.indexOf(item->getItem().gallery) < 0) {
                galleries.append(item->getItem().gallery);
            }
        }
        foreach(GalleryData* item, galleries) {
            emit GalleryHandler::getInstance()->onUpdGallery(item);
        }

        customGalleries.removeOne(&value);

        emit onDelCustomGallery(&value);
        delete &value;
        return true;
    }
    return false;
}

bool CustomGalleryHandler::updCustomGalleryItemName(CustomGalleryItemData& item, const QString& name)
{
    if(item.getName() != name && DbHandler::getInstance()->updCustomGalleryItemName(item, name)) {
        item.setName(name);
        foreach(CustomGalleryItemData* i, item.getChildren()) {
            if(DbHandler::getInstance()->updCustomGalleryItemName(*i, name)) {
                i->setName(name);
            }
        }

        emit onUpdCustomGalleryItem(&item);
        return true;
    }
    return false;
}


bool CustomGalleryHandler::updCustomGalleryItemCustomId(CustomGalleryItemData& item, int customId)
{
    if(item.getCustomId() != customId && DbHandler::getInstance()->updCustomGalleryItemCustomId(item, customId)) {
        item.setCustomId(customId);

        CustomGalleryItemData* i = getCustomGalleryItemById(item.getCustomGallery()->getId(), item.getCustomId());
        if(item.getCustomId() != 0) {
            i->children.append(&item);

            if(DbHandler::getInstance()->updCustomGalleryItemName(item, i->getName())) {
                item.setName(i->getName());
            }

        } else {
            i->children.removeOne(&item);
        }

        emit onUpdCustomGalleryItem(&item);
        return true;
    }
    return false;
}

bool CustomGalleryHandler::updCustomGalleryItemAngle(CustomGalleryItemData& item, int angle)
{
    if(item.getAngle() != angle && DbHandler::getInstance()->updCustomGalleryItemAngle(item, angle)) {
        item.setAngle(angle);

        emit onUpdCustomGalleryItemAngle(&item);
        return true;
    }
    return false;
}

CustomGalleryData* CustomGalleryHandler::updCustomGallery(CustomGalleryData& value)
{
    if(DbHandler::getInstance()->updCustomGallery(value)) {
        CustomGalleryData* gallery = getCustomGalleryById(value.getId());
        gallery->setName(value.getName());

        emit onUpdCustomGallery(gallery);
        return gallery;
    }
    return NULL;
}

CustomGalleryItemData* CustomGalleryHandler::getCustomGalleryItemById(int galleryId, int id) const
{
    foreach(CustomGalleryData* gallery, customGalleries) {
        if(gallery->getId() == galleryId) {
            foreach(CustomGalleryItemData* item, gallery->getItems()) {
                if(item->getId() == id) {
                    return item;
                }
            }
            break;
        }
    }
    return NULL;
}

CustomGalleryItemData* CustomGalleryHandler::getCustomGalleryItemById(int id) const
{
    foreach(CustomGalleryData* gallery, customGalleries) {
        foreach(CustomGalleryItemData* item, gallery->getItems()) {
            if(item->getItem().getId() == id) {
                return item;
            }
        }
    }
    return NULL;
}

CustomGalleryData* CustomGalleryHandler::getCustomGalleryById(int id) const
{
    foreach(CustomGalleryData* gallery, customGalleries) {
        if(gallery->getId() == id) {
            return gallery;
        }
    }
    return NULL;
}
