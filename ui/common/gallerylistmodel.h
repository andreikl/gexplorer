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

class GalleryListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    GalleryListModel(CustomGalleryData& gallery, int size, QObject* parent);
    GalleryListModel(GalleryData& gallery, int size, QObject* parent);
    virtual ~GalleryListModel();

public:
    Config::ItemTypeEnum getType() const;
    QSortFilterProxyModel* getProxy();

public:
    QModelIndex getIndexByItem(CustomGalleryItemData* item);
    QModelIndex getIndexByItem(GalleryItemData* item);
    QModelIndex getCurrentItem(int row) const;
    QModelIndex getBackItem(int row) const;
    QModelIndex getNextItem(int row) const;

public:
    void changeItemsSize(int size);

// QAbstractListModel
public:
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

private slots:
    void delGalleryItemEvent(GalleryItemData* item);

    void updCustomGalleryItemAngleEvent(CustomGalleryItemData* item);
    void updCustomGalleryItemEvent(CustomGalleryItemData* item);
    void addCustomGalleryItemEvent(CustomGalleryItemData* item);
    void delCustomGalleryItemEvent(CustomGalleryItemData* item);

    void loadEvent();

private:
    GalleryListItem* getItemByGalleryItem(CustomGalleryItemData* item);
    GalleryListItem* getItemByGalleryItem(GalleryItemData* item);

private:
    Config::ItemTypeEnum type;
    void* gallery;
    int size;

private:
    QList<GalleryListItem*> items;
    QSortFilterProxyModel* proxy;

};

#endif // GALLERYLISTMODEL_H
