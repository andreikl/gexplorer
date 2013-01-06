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
    GalleryTreeItem(BaseData* data, const QString& name, GalleryTreeItem* parent);
    virtual ~GalleryTreeItem();

public:
    CustomGalleryData* getCustomGallery();
    GalleryData* getGallery();
    KeyData* getKey();

public:
    GalleryTreeItem* getParent();
    const QString& getName();
    BaseData* getData();

public:
    void setParent(GalleryTreeItem* parent);
    void setName(const QString& name);
    void setData(BaseData* data);

public:
    QList<GalleryTreeItem*>& getChilds();

private:
    GalleryTreeItem* parent;
    BaseData* data;
    QString name;

private:
    QList<GalleryTreeItem*> childs;

};

Q_DECLARE_METATYPE(GalleryTreeItem*)

#endif // GALLERYTREEITEM_H
