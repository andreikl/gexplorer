#include <QtCore/QDebug>

#include "config.h"

#include "data/galleryitemdata.h"
#include "data/extensiondata.h"
#include "data/gallerydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/downloadhandler.h"
#include "handlers/galleryhandler.h"
#include "handlers/keyhandler.h"
#include "handlers/dbhandler.h"

#include "application.h"

Application* Application::app = NULL;

Application* Application::getInstance()
{
    return app;
}

Application* Application::createInstance(const QString& path)
{
    if(!app) {
        Application* tempApp = new Application();

        DbHandler* db = DbHandler::createInstance(path);
        bool res = db;
        res && (res = db->getExtensions(tempApp->extensions));
        res && (res = GalleryHandler::createInstance(tempApp));
        res && (res = CustomGalleryHandler::createInstance(tempApp));
        res && (res = KeyHandler::createInstance(tempApp));
        if(res) {
            foreach(GalleryData* gallery, GalleryHandler::getInstance()->getGalleries()) {
                foreach(GalleryItemData* item, gallery->getItems()) {
                    if(item->getStatus() == GalleryItemData::StatusDownload) {
                        tempApp->addToDownload(*item);
                    }
                }
            }
        }
        if(res) {
            app = tempApp;
        } else {
            delete tempApp;
        }
    }
    return app;
}

Application::Application(): activeDownload(0)
{
}

Application::~Application()
{
    app = NULL;
    foreach(DownloadHandler* downloadHandler, queue) {
        delete downloadHandler;
    }
    if(KeyHandler::getInstance())
    {
        delete KeyHandler::getInstance();
    }
    if(CustomGalleryHandler::getInstance())
    {
        delete CustomGalleryHandler::getInstance();
    }
    if(GalleryHandler::getInstance())
    {
        delete GalleryHandler::getInstance();
    }
    foreach(ExtensionData* pExtension, extensions) {
        delete pExtension;
    }
    if(DbHandler::getInstance())
    {
        delete DbHandler::getInstance();
    }
}

ExtensionData* Application::getExtensionByName(const QString& name) const
{
    foreach(ExtensionData* extension, extensions) {
        if(extension->getName().compare(name, Qt::CaseInsensitive) == 0) {
            return extension;
        }
    }
    return NULL;
}

const QQueue<DownloadHandler*>& Application::getDownloaders() const
{
    return queue;
}

const QList<ExtensionData*>& Application::getExtensions() const
{
    return extensions;
}

ExtensionData* Application::getExtensionById(int id) const
{
    foreach(ExtensionData* extension, extensions) {
        if(extension->getId() == id) {
            return extension;
        }
    }
    return NULL;
}

void Application::addToDownload(GalleryItemData& value)
{
    DownloadHandler* downloader = new DownloadHandler(value);
    connect(downloader, SIGNAL(onFinish(DownloadHandler*, bool)), SLOT(finishEvent(DownloadHandler*, bool)));

    queue.enqueue(downloader);
    emit onDownloadEnqueue(downloader);
    if(activeDownload < Config::Self()->ActiveDownloadCount()) {
        if(downloader->load()) {
            activeDownload++;
        } else {
            value.setStatus(GalleryItemData::StatusError);
            if(!DbHandler::getInstance()->updGalleryItemStatus(value)) {
            }
            queue.removeAt(queue.indexOf(downloader));
            emit onDownloadDequeue(downloader);
            delete downloader;
        }
    }
}

void Application::finishEvent(DownloadHandler* sender, bool res)
{
    GalleryItemData::GalleryItemStatusEnum status = (res)? GalleryItemData::StatusComplete: GalleryItemData::StatusError;
    GalleryItemData* item = GalleryHandler::getInstance()->getGalleryItemById(sender->getItem()->getId());
    if(item != NULL) {
        item->setStatus(status);
        if(!DbHandler::getInstance()->updGalleryItemStatus(*item)) {
        }
    }

    queue.removeAt(queue.indexOf(sender));
    emit onDownloadDequeue(sender);

    delete sender;
    activeDownload--;

    if(activeDownload < Config::Self()->ActiveDownloadCount()) {
        for(int i = 0; i < queue.count(); i++) {
            DownloadHandler* downloader = queue[i];
            if(!downloader->isDownload()) {
                if(downloader->load()) {
                    activeDownload++;
                    break;
                } else {
                    item = GalleryHandler::getInstance()->getGalleryItemById(downloader->getItem()->getId());
                    if(item != NULL) {
                        item->setStatus(GalleryItemData::StatusError);
                        if(!DbHandler::getInstance()->updGalleryItemStatus(*item)) {
                        }
                    }
                    queue.removeAt(queue.indexOf(downloader));
                    emit onDownloadDequeue(downloader);
                    delete downloader;
                }
            }
        }
    }
}
