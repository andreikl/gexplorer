#include <QtCore/QStringList>
#include <QtCore/QDir>

#include "application.h"

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"
#include "data/gallerydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/dbhandler.h"

#include "handlers/galleryhandler.h"

GalleryHandler* GalleryHandler::handler = NULL;

GalleryHandler* GalleryHandler::createInstance(Application* app)
{
    if(!handler) {
        GalleryHandler* tempHandler = new GalleryHandler();
        if(DbHandler::getInstance()->getGalleries(app, tempHandler->galleries)) {
            handler = tempHandler;
        } else {
            delete tempHandler;
        }
    }
    return handler;
}

GalleryHandler* GalleryHandler::getInstance()
{
    return handler;
}

GalleryHandler::~GalleryHandler()
{
    handler = NULL;
    foreach(GalleryData* gallery, galleries) {
        delete gallery;
    }
}

GalleryHandler::GalleryHandler()
{
}

const QList<GalleryData*>& GalleryHandler::getGalleries() const
{
    return galleries;
}

GalleryData* GalleryHandler::getGalleryById(int id) const
{
    foreach(GalleryData* gallery, galleries) {
        if(gallery->getId() == id) {
            return gallery;
        }
    }
    return NULL;
}

void GalleryHandler::addFileGallery(const QString& source)
{
    QStringList filters;
    foreach(const ExtensionData* extension, Application::getInstance()->getExtensions()) {
        filters.append("*" + extension->getName());
    }

    QDir dir(source);
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);

    if(files.count()) {
        GalleryData* gallery = new GalleryData(GalleryData::FileGallery);
        gallery->setSource(source);
        gallery->setPath(dir.path());

        foreach(const QString& file, files) {
            int i = file.lastIndexOf('.');
            if(i == -1) {
                continue;
            }
            QString extension = file.right(file.length() - i);
            ExtensionData* e = Application::getInstance()->getExtensionByName(extension);
            if(e == NULL) {
                continue;
            }
            GalleryItemData* item = new GalleryItemData(gallery, *e);
            item->setStatus(GalleryItemData::StatusComplete);
            item->setUrl(dir.path() + QDir::separator() + file);
            item->setFileName(file);
            item->setPath(QDir("").path());
        }

        if(DbHandler::getInstance()->addGallery(*gallery)) {
            galleries.append(gallery);
            emit onAddGallery(gallery);
        } else {
            delete gallery;
        }
    }
}

void GalleryHandler::addWebGallery(const GalleryData& value)
{
    GalleryData* gallery = value.clone();
    foreach(GalleryItemData* item, gallery->getItems()) {
        item->setStatus(GalleryItemData::StatusDownload);
        if(!DbHandler::getInstance()->updGalleryItemStatus(*item)) {
        }
    }
    if(DbHandler::getInstance()->addGallery(*gallery)) {
        galleries.append(gallery);
        emit onAddGallery(gallery);
        foreach(GalleryItemData* item, gallery->getItems()) {
            Application::getInstance()->addToDownload(*item);
        }
    } else {
        delete gallery;
    }
}

bool GalleryHandler::delGallery(GalleryData& value)
{
    if(DbHandler::getInstance()->delGallery(value)) {
        foreach(GalleryItemData* item, value.getItems()) {
            foreach(CustomGalleryData* customGallery, CustomGalleryHandler::getInstance()->getCustomGalleries()) {
                foreach(CustomGalleryItemData* customItem, customGallery->getItems()) {
                    if(&customItem->getItem() == item) {
                        foreach(CustomGalleryItemData* i, customItem->getChildren()) {
                            i->setCustomId(0);
                            emit CustomGalleryHandler::getInstance()->onUpdCustomGalleryItem(i);
                        }
                        emit CustomGalleryHandler::getInstance()->onDelCustomGalleryItem(customItem);
                        delete customItem;
                    }
                }
            }
        }
        galleries.removeOne(&value);

        emit onDelGallery(&value);
        delete &value;
        return true;
    }
    return false;
}

GalleryItemData* GalleryHandler::getGalleryItemById(int galleryId, int id) const
{
    foreach(GalleryData* gallery, galleries) {
        if(gallery->getId() == galleryId) {
            foreach(GalleryItemData* item, gallery->getItems()) {
                if(item->getId() == id) {
                    return item;
                }
            }
            break;
        }
    }
    return NULL;
}

GalleryItemData* GalleryHandler::getGalleryItemById(int id) const
{
    foreach(GalleryData* gallery, galleries) {
        foreach(GalleryItemData* item, gallery->getItems()) {
            if(item->getId() == id) {
                return item;
            }
        }
    }
    return NULL;
}

