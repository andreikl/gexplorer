#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QDir>

#include "application.h"

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"
#include "data/gallerydata.h"

#include "helpers/commonhelper.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/dbhandler.h"

#include "handlers/galleryhandler.h"

GalleryHandler* GalleryHandler::handler = NULL;

GalleryHandler* GalleryHandler::createInstance(Application* app)
{
    if(!handler) {
        GalleryHandler* tempHandler = new GalleryHandler();
        bool res = DbHandler::getInstance()->getGalleries(tempHandler->galleries);
        res && (res = DbHandler::getInstance()->getGalleryItems(app, tempHandler));
        if(res) {
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

    QDir dir(QDir("").relativeFilePath(source));
    QStringList files = dir.entryList(filters, QDir::Files, QDir::Name);

    if(files.count()) {
        GalleryData* oldgallery = getGalleryByPath(dir.path());

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

            QString url = dir.path() + QDir::separator() + file;
            GalleryItemData* item = NULL;
            if(oldgallery) {
                item = getGalleryItemByUrl(oldgallery, url);
            }
            if(!item) {
                item = new GalleryItemData(gallery, *e);
                item->setStatus(GalleryItemData::StatusComplete);
                item->setUrl(url);
                item->setFileName(file);
                item->setPath(QDir("").path());
            }
        }

        if(oldgallery) {
            gallery->setId(oldgallery->getId());
            if(DbHandler::getInstance()->addToGallery(*gallery)) {
                foreach(GalleryItemData* item, gallery->getItems()) {
                    item->clone(oldgallery);
                }
                emit onUpdGallery(oldgallery);
            }
            delete gallery;
        } else {
            if(DbHandler::getInstance()->addGallery(*gallery)) {
                galleries.append(gallery);
                emit onAddGallery(gallery);
            } else {
                delete gallery;
            }
        }
    }
}

void GalleryHandler::addToWebGallery(const GalleryData& value)
{
    GalleryData* gallery = value.clone();
    foreach(GalleryItemData* item, gallery->getItems()) {
        item->setStatus(GalleryItemData::StatusDownload);
        /*if(!DbHandler::getInstance()->updGalleryItemStatus(*item)) {
        }*/
    }
    if(DbHandler::getInstance()->addToGallery(*gallery)) {
        GalleryData* g = getGalleryById(value.getId());
        foreach(GalleryItemData* item, gallery->getItems()) {
            GalleryItemData* i = item->clone(g);
            Application::getInstance()->addToDownload(*i);
        }
        emit onUpdGallery(g);
    }
    delete gallery;
}

void GalleryHandler::addWebGallery(const GalleryData& value)
{
    GalleryData* gallery = value.clone();
    foreach(GalleryItemData* item, gallery->getItems()) {
        item->setStatus(GalleryItemData::StatusDownload);
        /*if(!DbHandler::getInstance()->updGalleryItemStatus(*item)) {
        }*/
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

bool GalleryHandler::delFromGallery(GalleryItemData& value)
{
    foreach(CustomGalleryItemData* item, value.customItems) {
        if(item->getChildren().count() > 0) {
            qDebug() << "you must delete child custom items before";
            return false;
        }
    }

    if(DbHandler::getInstance()->delFromGallery(value)) {
        foreach(CustomGalleryItemData* item, value.customItems) {
            if(item->getCustomId()) {
                CustomGalleryItemData* i = CustomGalleryHandler::getInstance()->getCustomGalleryItemById(item->gallery->getId(), item->getCustomId());
                i->children.removeOne(item);

                emit CustomGalleryHandler::getInstance()->onUpdCustomGalleryItem(i);
            }

            emit CustomGalleryHandler::getInstance()->onDelCustomGalleryItem(item);
            delete item;
        }

        emit onDelGalleryItem(&value);
        delete &value;

        return true;
    }
    return false;
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

bool GalleryHandler::updGalleryItemReference(const CustomGalleryItemData& item, const CustomGalleryItemData& reference)
{
    GalleryItemData* gitem = &item.item;
    GalleryItemData* greference = &reference.item;
    int angle = reference.getAngle() - item.getAngle();

    if(!gitem->getUrl().startsWith("http://", Qt::CaseInsensitive)) {
        qDebug() << "changing local gallery isn't allowed";
        return false;
    }

    if(greference->getReference()) {
        qDebug() << "reference is already reference";
        return false;
    }

    if(DbHandler::getInstance()->updGalleryItemReference(*gitem, *greference, angle)) {
        gitem->reference = greference;
        gitem->referenceAngle = angle;

        const QUrl& galleryPath = QUrl::fromLocalFile(gitem->getGallery()->getPath() + QDir::separator());
        const QUrl& path = galleryPath.resolved(QUrl::fromLocalFile(gitem->getPath()));
        QDir dir(path.toLocalFile());

        //qDebug() << "url path: " << path.toLocalFile();
        qDebug() << "dir path: " << dir.path();
        qDebug() << "dir name: " << dir.dirName();

        dir.remove(gitem->fileName);
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

        while(dir.count() == 0) {
            const QString dirname = dir.dirName();
            if(!dir.cdUp()) {
                break;
            }
            dir.rmdir(dirname);
        }
        emit onUpdGalleryItem(gitem);
        return true;
    }
    return false;
}

GalleryData* GalleryHandler::getGalleryByPath(const QString& path) const
{
    foreach(GalleryData* gallery, galleries) {
        if(gallery->getPath().compare(path, Qt::CaseInsensitive) == 0) {
            return gallery;
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

GalleryItemData* GalleryHandler::getGalleryItemByUrl(GalleryData* gallery, const QString& url) const
{
    foreach(GalleryItemData* item, gallery->getItems()) {
        if(item->getUrl().compare(url, Qt::CaseInsensitive) == 0) {
            return item;
        }
    }
    return NULL;
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
