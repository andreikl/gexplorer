
#include "data/customgallerydata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/dbhandler.h"

#include "handlers/keyhandler.h"

KeyHandler* KeyHandler::handler = NULL;

KeyHandler* KeyHandler::createInstance(Application*)
{
    if(!handler) {
        KeyHandler* tempHandler = new KeyHandler();
        if(DbHandler::getInstance()->getKeys(tempHandler->keys)) {
            foreach(KeyData* key, tempHandler->getKeys()) {
                foreach(CustomGalleryData* gallery, key->getGalleries()) {
                    gallery->keys.append(key);
                }
            }
            handler = tempHandler;
        } else {
            delete tempHandler;
        }
    }
    return handler;
}

KeyHandler* KeyHandler::getInstance()
{
    return handler;
}

KeyHandler::KeyHandler()
{
}

KeyHandler::~KeyHandler()
{
    handler = NULL;
    foreach(KeyData* key, keys) {
        delete key;
    }
}

const QList<KeyData*>& KeyHandler::getKeys() const
{
    return keys;
}

CustomGalleryData* KeyHandler::addToKey(KeyData& key, int galleryId)
{
    CustomGalleryData* gallery = CustomGalleryHandler::getInstance()->getCustomGalleryById(galleryId);
    if(gallery && DbHandler::getInstance()->addToKey(key, *gallery)) {
        key.galleries.append(gallery);

        gallery->keys.append(&key);
        emit CustomGalleryHandler::getInstance()->onUpdCustomGallery(gallery);
        emit onUpdKey(&key);

        return gallery;
    }
    return NULL;
}

KeyData* KeyHandler::addKey(const KeyData& value)
{
    KeyData* key = value.clone();
    if(DbHandler::getInstance()->addKey(*key)) {
        keys.append(key);

        emit onAddKey(key);
        return key;
    } else {
        delete key;
    }
    return NULL;
}

bool KeyHandler::delFromKey(KeyData& key, CustomGalleryData& value)
{
    if(DbHandler::getInstance()->delFromKey(key, value)) {
        key.galleries.removeOne(&value);

        value.keys.removeOne(&key);
        emit CustomGalleryHandler::getInstance()->onUpdCustomGallery(&value);
        emit onUpdKey(&key);

        return true;
    }
    return false;
}

bool KeyHandler::delKey(KeyData& value)
{
    if(DbHandler::getInstance()->delKey(value)) {
        keys.removeOne(&value);

        emit onDelKey(&value);
        delete &value;
        return true;
    }
    return false;
}

KeyData* KeyHandler::updKey(KeyData& value)
{
    if(DbHandler::getInstance()->updKey(value)) {
        KeyData* key = getKeyById(value.getId());
        key->setName(value.getName());

        emit onUpdKey(key);
        return key;
    }
    return NULL;
}

KeyData* KeyHandler::getKeyById(int id) const
{
    foreach(KeyData* key, keys) {
        if(key->getId() == id) {
            return key;
        }
    }
    return NULL;
}

