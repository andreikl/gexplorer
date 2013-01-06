#define MAX_ICON_SIZE 300

#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"

#include "ui/common/gallerylistitem.h"

#include "ui/common/gallerylistmodel.h"

GalleryListModel::GalleryListModel(CustomGalleryData& gallery, int size, QObject* parent): QAbstractListModel(parent), gallery(NULL), proxy(NULL)
{
    type = Config::ETypeCustomGallery;
    this->gallery = &gallery;
    this->size = size;

    foreach(CustomGalleryItemData* item, gallery.getItems()) {
        if(item->getCustomId() == 0) {
            items.append(new GalleryListItem(*item, size));
        }
    }

    proxy = new QSortFilterProxyModel(parent);
    proxy->setSourceModel(this);
    proxy->sort(0);

    connect(CustomGalleryHandler::getInstance(), SIGNAL(onUpdCustomGalleryItemAngle(CustomGalleryItemData*)), this, SLOT(updCustomGalleryItemAngleEvent(CustomGalleryItemData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onUpdCustomGalleryItem(CustomGalleryItemData*)), this, SLOT(updCustomGalleryItemEvent(CustomGalleryItemData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onAddCustomGalleryItem(CustomGalleryItemData*)), this, SLOT(addCustomGalleryItemEvent(CustomGalleryItemData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onDelCustomGalleryItem(CustomGalleryItemData*)), this, SLOT(delCustomGalleryItemEvent(CustomGalleryItemData*)));

    QTimer::singleShot(100, this, SLOT(loadEvent()));
}

GalleryListModel::GalleryListModel(GalleryData& gallery, int size, QObject* parent): QAbstractListModel(parent), gallery(NULL), proxy(NULL)
{
    type = Config::ETypeGallery;
    this->gallery = &gallery;
    this->size = size;

    foreach(GalleryItemData* item, gallery.getItems()) {
        items.append(new GalleryListItem(*item, size));
    }

    proxy = new QSortFilterProxyModel(parent);
    proxy->setSourceModel(this);
    proxy->sort(0);

    connect(GalleryHandler::getInstance(), SIGNAL(onDelGalleryItem(GalleryItemData*)), this, SLOT(delGalleryItemEvent(GalleryItemData*)));

    QTimer::singleShot(100, this, SLOT(loadEvent()));
}

GalleryListModel::~GalleryListModel()
{
    if(proxy) {
        delete proxy;
        proxy = NULL;
    }

    qDeleteAll(items);
}

Config::ItemTypeEnum GalleryListModel::getType() const
{
    return type;
}

QSortFilterProxyModel* GalleryListModel::getProxy()
{
    return proxy;
}

QModelIndex GalleryListModel::getIndexByItem(CustomGalleryItemData* item)
{
    while(item->getCustomId()) {
        foreach(CustomGalleryItemData* i, item->getCustomGallery()->getItems()) {
            if(item->getCustomId() == i->getId()) {
                item = i;
                break;
            }
        }
    }
    GalleryListItem* i = getItemByGalleryItem(item);
    return proxy->mapFromSource(index(items.indexOf(i), 0));
}

QModelIndex GalleryListModel::getIndexByItem(GalleryItemData* item)
{
    GalleryListItem* i = getItemByGalleryItem(item);
    return proxy->mapFromSource(index(items.indexOf(i), 0));
}

QModelIndex GalleryListModel::getCurrentItem(int row) const
{
    return proxy->index(row, 0);
}

QModelIndex GalleryListModel::getBackItem(int row) const
{
    return proxy->index(row - 1, 0);
}

QModelIndex GalleryListModel::getNextItem(int row) const
{
    return proxy->index(row + 1, 0);
}

void GalleryListModel::changeItemsSize(int size)
{
    if(this->size != size) {
        this->size = size;

        beginRemoveRows(QModelIndex(), 0, items.count() - 1);
        for(int i = 0; i < items.count(); i++) {
            delete items.at(i);
        }
        items.clear();
        endRemoveRows();

        QTimer::singleShot(1, this, SLOT(loadEvent()));
    }
}

QVariant GalleryListModel::data(const QModelIndex& index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return QVariant(items[index.row()]->getName());

    case Qt::DecorationRole:
        return QVariant(items[index.row()]->getPixmap());

    case Qt::UserRole:
        return qVariantFromValue(items[index.row()]);

    case Qt::UserRole + 1:
        if(type == Config::ETypeGallery) {
            return qVariantFromValue(reinterpret_cast<GalleryItemData*>(items.at(index.row())->getItem()));
        } else if(type == Config::ETypeCustomGallery) {
            return qVariantFromValue(reinterpret_cast<CustomGalleryItemData*>(items.at(index.row())->getItem()));
        }
    }
    return QVariant();
}

Qt::ItemFlags GalleryListModel::flags(const QModelIndex& index) const
{
        return QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

int GalleryListModel::rowCount(const QModelIndex& index) const
{
    (void)index;
    return items.count();
}

void GalleryListModel::delGalleryItemEvent(GalleryItemData* item)
{
    GalleryData* g = reinterpret_cast<GalleryData*>(gallery);
    if(g == item->getGallery()) {
        GalleryListItem* i = getItemByGalleryItem(item);
        if(i) {
            int index = items.indexOf(i);

            beginRemoveRows(QModelIndex(), index, index);
            delete items.at(index);
            items.removeAt(index);
            endRemoveRows();
        }
    }
}

void GalleryListModel::updCustomGalleryItemAngleEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(gallery);
    if(g == item->getCustomGallery()) {
        GalleryListItem* i = getItemByGalleryItem(item);
        i->loadPixmap(*item, size);
        const QModelIndex& in = getIndexByItem(item);
        emit dataChanged(in, in);

        //const QModelIndex& i = getIndexByItem(item);
        /*delCustomGalleryItemEvent(item);
        addCustomGalleryItemEvent(item);
        proxy->sort(0);*/
    }
}

void GalleryListModel::updCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(gallery);
    if(g == item->getCustomGallery()) {
        GalleryListItem* i = getItemByGalleryItem(item);
        if(i == NULL && item->getCustomId() == 0) {
            addCustomGalleryItemEvent(item);
            proxy->sort(0);
        } else if(i != NULL && item->getCustomId() != 0) {
            delCustomGalleryItemEvent(item);
        } else if(i != NULL) {
            const QModelIndex& in = index(items.indexOf(i));
            dataChanged(in, in);
        }
    }
}

void GalleryListModel::addCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(gallery);
    if(g == item->getCustomGallery()) {
        beginInsertRows(QModelIndex(), items.count(), items.count());
        items.append(new GalleryListItem(*item, size));
        endInsertRows();
    }
}

void GalleryListModel::delCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(gallery);
    if(g == item->getCustomGallery()) {
        GalleryListItem* i = getItemByGalleryItem(item);
        if(i) {
            int index = items.indexOf(i);

            beginRemoveRows(QModelIndex(), index, index);
            delete items.at(index);
            items.removeAt(index);
            endRemoveRows();
        }
    }
}

void GalleryListModel::loadEvent()
{
    foreach(GalleryListItem* item, items) {
        if(!item->isLoad) {
            item->loadPixmap(size);
            const QModelIndex& in = proxy->mapFromSource(index(items.indexOf(item), 0));
            emit dataChanged(in, in);

            QTimer::singleShot(1, this, SLOT(loadEvent()));
            break;
        }
     }

    /*if(type == Config::ETypeGallery) {
        GalleryData* g = reinterpret_cast<GalleryData*>(gallery);
        foreach(GalleryItemData* item, g->getItems()) {
            if(getItemByGalleryItem(item) == NULL) {
                beginInsertRows(QModelIndex(), items.count(), items.count());
                items.append(new GalleryListItem(*item, size));
                endInsertRows();

                QTimer::singleShot(1, this, SLOT(loadEvent()));
                break;
            }
        }
    } else if(type == Config::ETypeCustomGallery) {
        CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(gallery);
        foreach(CustomGalleryItemData* item, g->getItems()) {
            if(item->getCustomId() == 0 && getItemByGalleryItem(item) == NULL) {
                beginInsertRows(QModelIndex(), items.count(), items.count());
                items.append(new GalleryListItem(*item, size));
                endInsertRows();

                QTimer::singleShot(1, this, SLOT(loadEvent()));
                break;
            }
        }
    }
    proxy->sort(0);*/
}

GalleryListItem* GalleryListModel::getItemByGalleryItem(CustomGalleryItemData* item)
{
    foreach(GalleryListItem* i, items) {
        if(reinterpret_cast<CustomGalleryItemData*>(i->getItem()) == item) {
            return i;
        }
    }
    return NULL;
}

GalleryListItem* GalleryListModel::getItemByGalleryItem(GalleryItemData* item)
{
    foreach(GalleryListItem* i, items) {
        if(reinterpret_cast<GalleryItemData*>(i->getItem()) == item) {
            return i;
        }
    }
    return NULL;
}
