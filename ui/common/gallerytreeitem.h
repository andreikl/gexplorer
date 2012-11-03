#ifndef GALLERYTREEITEM_H
#define GALLERYTREEITEM_H

#include <QtCore/QMetaType>

class CustomGalleryData;
class GalleryData;
class BaseData;
class KeyData;

class GalleryTreeItem
{
public:
    GalleryTreeItem(BaseData* pData, const QString& name, GalleryTreeItem* pParent);
    virtual ~GalleryTreeItem();

public:
    GalleryData* Gallery();
    CustomGalleryData* CustomGallery();
    KeyData* Key();

public:
    BaseData* Data();
    void Data(BaseData* pData);

public:
    const QString& Name();
    void Name(const QString& name);

public:
    GalleryTreeItem* Parent();
    void Parent(GalleryTreeItem* pParent);

public:
    QList<GalleryTreeItem*>& Childs();

private:
    QString name;

private:
    BaseData* pData;

private:
    QList<GalleryTreeItem*> childs;
    GalleryTreeItem* pParent;
};


Q_DECLARE_METATYPE(GalleryTreeItem*)

#endif // GALLERYTREEITEM_H
