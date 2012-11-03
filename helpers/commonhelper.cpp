#include <QtCore/QUrl>
#include <QtCore/QDir>

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "helpers/commonhelper.h"

QString CommonHelper::getPath(const GalleryItemData& item)
{
    QUrl path = QUrl::fromLocalFile(item.getGallery()->getPath() + QDir::separator());
    path = path.resolved(QUrl::fromLocalFile(item.getPath() + QDir::separator()));
    path = path.resolved(QUrl::fromLocalFile(item.getFileName()));
    QString p = path.toLocalFile();
    if(path.isRelative()) {
        p = QDir::currentPath() + QDir::separator() + p;
    }
    return p;
}

QString CommonHelper::getPath(const CustomGalleryItemData& item)
{
    QUrl path = QUrl::fromLocalFile(item.getItem().getGallery()->getPath() + QDir::separator());
    path = path.resolved(QUrl::fromLocalFile(item.getItem().getPath() + QDir::separator()));
    path = path.resolved(QUrl::fromLocalFile(item.getItem().getFileName()));
    QString p = path.toLocalFile();
    if(path.isRelative()) {
        p = QDir::currentPath() + QDir::separator() + p;
    }
    return p;
}
