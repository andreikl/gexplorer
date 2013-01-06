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

CustomGalleryHandler* CustomGalleryHandler::createInstance()
{
    if(!handler) {
        CustomGalleryHandler* tempHandler = new CustomGalleryHandler();
        bool res = DbHandler::getInstance()->getCustomGalleries(tempHandler->customGalleries);
        res && (res = DbHandler::getInstance()->getCustomGalleryItems(tempHandler));
        if(res) {
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

QList<CustomGalleryItemData*> CustomGalleryHandler::addToCustomGallery(CustomGalleryData& value, QList<CustomGalleryItemId>& ids)
{
    QList<CustomGalleryItemData*> items;
    foreach(const CustomGalleryItemId& id, ids) {
        GalleryItemData* item = GalleryHandler::getInstance()->getGalleryItemById(id.gid, id.id);
        bool isExist = false;

        foreach(CustomGalleryItemData* i, value.getItems()) {
            if(i->item.id == id.id) {
                isExist = true;
                break;
            }
        }

        if(!isExist) {
            CustomGalleryItemData* i = new CustomGalleryItemData(value, *item);
            i->setName(item->getFileName());
            items.append(i);
        }
    }

    if(DbHandler::getInstance()->addToCustomGallery(items)) {
        QList<GalleryData*> galleries;
        foreach(CustomGalleryItemData* i, items) {
            i->getItem().customItems.append(i);
            emit onAddCustomGalleryItem(i);
            if(!galleries.contains(i->getItem().gallery)) {
                galleries.append(i->getItem().gallery);
            }
        }
        foreach(GalleryData* g, galleries) {
            emit GalleryHandler::getInstance()->onUpdGallery(g);
        }
    } else {
        foreach(CustomGalleryItemData* i, items) {
            delete i;
        }
        items.clear();
    }
    return items;
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
        //QList<KeyData*> keys;
        foreach(KeyData* key, value.getKeys()) {
            key->galleries.removeOne(&value);
            emit KeyHandler::getInstance()->onDelFromKey(key, &value);
            /*if(keys.indexOf(key) < 0) {
                keys.append(key);
            }*/
        }
        /*foreach(KeyData* item, keys) {
            emit KeyHandler::getInstance()->onUpdKey(item);
        }*/
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

bool CustomGalleryHandler::updCustomGalleryItemNames(QList<CustomGalleryItemName>& names)
{
    if(DbHandler::getInstance()->updCustomGalleryItemNames(names)) {
        foreach(const CustomGalleryItemName& item, names) {
            item.item->setName(item.name);
            foreach(CustomGalleryItemData* i, item.item->getChildren()) {
                i->setName(item.name);
            }

            emit onUpdCustomGalleryItem(item.item);
        }
        return true;
    }
    return false;
}

bool CustomGalleryHandler::updCustomGalleryItemUnite(CustomGalleryItemData& item, int customId)
{
    CustomGalleryItemData* i = getCustomGalleryItemById(item.getCustomGallery()->getId(), customId);
    if(i && item.getId() != customId && DbHandler::getInstance()->updCustomGalleryItemUnite(item, customId)) {
        item.setCustomId(customId);

        i->children.append(&item);

        if(DbHandler::getInstance()->updCustomGalleryItemName(item, i->getName())) {
            item.setName(i->getName());
        }
        emit onUpdCustomGalleryItem(&item);
        return true;
    }
    return false;
}

bool CustomGalleryHandler::updCustomGalleryItemSplit(CustomGalleryItemData& item)
{
    if(DbHandler::getInstance()->updCustomGalleryItemSplit(item)) {
        foreach(CustomGalleryItemData* i, item.getChildren()) {
            i->setCustomId(0);
            emit onAddCustomGalleryItem(i);
        }
        item.children.clear();

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
    CustomGalleryData* gallery = getCustomGalleryById(value.getId());
    if(gallery->getName().compare(value.getName()) != 0) {
        if(DbHandler::getInstance()->updCustomGallery(value)) {
            gallery->setName(value.getName());
            emit onUpdCustomGallery(gallery, "name");
        } else {
            gallery = NULL;
        }
    }
    return gallery;
}

CustomGalleryItemData* CustomGalleryHandler::getCustomGalleryItemById(int galleryId, int id)
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
