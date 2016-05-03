#include <QtCore/QUrl>
#include <QtCore/QDir>

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "helpers/commonhelper.h"

QString CommonHelper::getPath(const GalleryItemData& item)
{
    if(item.getReference() != NULL) {
        return getPath(*item.getReference());
    }

    QString p;
    QUrl path = QUrl::fromLocalFile(item.getGallery()->getPath() + QDir::separator());
    p = path.toLocalFile();

    path = QUrl::fromLocalFile(item.getPath() + QDir::separator() + item.getFileName());
    return p + QDir::separator() + path.toLocalFile();

    /*QString p;
    QUrl path = QUrl::fromLocalFile(item.getGallery()->getPath() + QDir::separator());
    path = path.resolved(QUrl::fromLocalFile(item.getPath() + QDir::separator()));
    path = path.resolved(QUrl::fromLocalFile(item.getFileName()));
    if(path.isRelative()) {
        p = QDir::currentPath() + QDir::separator() + path.toLocalFile();
    } else {
        p = path.toLocalFile();
    }*/
    return p;
}

int CommonHelper::getAngle(const GalleryItemData& item)
{
    if(item.getReference() != NULL) {
        return item.getReferenceAngle() + getAngle(*item.getReference());
    }
    return item.getReferenceAngle();
}

void CommonHelper::getChildren(const CustomGalleryItemData& item, QList<CustomGalleryItemData*>& children)
{
    foreach(CustomGalleryItemData* gi, item.getChildren()) {
        children.append(gi);
        getChildren(*gi, children);
    }
}

QString CommonHelper::getPath(const CustomGalleryItemData& item)
{
    return getPath(item.getItem());
}

int CommonHelper::getAngle(const CustomGalleryItemData& item)
{
    return item.getAngle();
}
