#ifndef GALLERYLISTMODEL_H
#define GALLERYLISTMODEL_H

#include <QtCore/QAbstractListModel>

#include "config.h"

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
QT_END_NAMESPACE

class CustomGalleryItemData;
class CustomGalleryData;
class GalleryListItem;
class GalleryItemData;
class GalleryData;

class GalleryListModel: public QAbstractListModel {
    Q_OBJECT

public:
    GalleryListModel(GalleryData& gallery, int size, QObject* pParent = NULL);
    GalleryListModel(CustomGalleryData& gallery, int size, QObject* pParent = NULL);
    virtual ~GalleryListModel();

public:
    Config::ItemTypeEnum Type() const;
    QSortFilterProxyModel* Proxy();

public:
    QModelIndex GetCurrentItem(int row) const;
    QModelIndex GetBackItem(int row) const;
    QModelIndex GetNextItem(int row) const;
    void ChangeItemsSize(int size);

// QAbstractListModel
public:
    Qt::ItemFlags flags(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private slots:
    void updCustomGalleryItemAngleEvent(CustomGalleryItemData* item);
    void updCustomGalleryItemEvent(CustomGalleryItemData* item);
    void addCustomGalleryItemEvent(CustomGalleryItemData* item);
    void delCustomGalleryItemEvent(CustomGalleryItemData* item);
    void loadEvent();

private:
    GalleryListItem* GetItemByGalleryItem(GalleryItemData* pItem);
    GalleryListItem* GetItemByGalleryItem(CustomGalleryItemData* pItem);

private:
    Config::ItemTypeEnum type;
    void* pGallery;
    int size;

private:
    QSortFilterProxyModel* pProxy;
    QList<GalleryListItem*> items;
};

#endif // GALLERYLISTMODEL_H
