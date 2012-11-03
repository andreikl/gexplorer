#define MAX_ICON_SIZE 300

#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include <QtGui/QSortFilterProxyModel>

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/customgalleryhandler.h"

#include "ui/common/gallerylistitem.h"

#include "ui/common/gallerylistmodel.h"

GalleryListModel::GalleryListModel(GalleryData& gallery, int size, QObject* pParent): QAbstractListModel(pParent), pGallery(NULL), pProxy(NULL) {
    type = Config::ETypeGallery;
    pGallery = &gallery;
    this->size = size;

    pProxy = new QSortFilterProxyModel(pParent);
    pProxy->setSourceModel(this);

    QTimer::singleShot(100, this, SLOT(loadEvent()));
}

GalleryListModel::GalleryListModel(CustomGalleryData& gallery, int size, QObject* pParent): QAbstractListModel(pParent), pGallery(NULL), pProxy(NULL) {
    type = Config::ETypeCustomGallery;
    pGallery = &gallery;
    this->size = size;

    pProxy = new QSortFilterProxyModel(pParent);
    pProxy->setSourceModel(this);

    connect(CustomGalleryHandler::getInstance(), SIGNAL(onUpdCustomGalleryItemAngle(CustomGalleryItemData*)), this, SLOT(updCustomGalleryItemAngleEvent(CustomGalleryItemData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onUpdCustomGalleryItem(CustomGalleryItemData*)), this, SLOT(updCustomGalleryItemEvent(CustomGalleryItemData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onAddCustomGalleryItem(CustomGalleryItemData*)), this, SLOT(addCustomGalleryItemEvent(CustomGalleryItemData*)));
    connect(CustomGalleryHandler::getInstance(), SIGNAL(onDelCustomGalleryItem(CustomGalleryItemData*)), this, SLOT(delCustomGalleryItemEvent(CustomGalleryItemData*)));

    QTimer::singleShot(100, this, SLOT(loadEvent()));
}

GalleryListModel::~GalleryListModel() {
    if(pProxy) {
        delete pProxy;
        pProxy = NULL;
    }

    qDeleteAll(items);
    /*foreach(GalleryListItem* pItem, items) {
        delete pItem;
    }*/
}

Config::ItemTypeEnum GalleryListModel::Type() const {
    return type;
}

QSortFilterProxyModel* GalleryListModel::Proxy() {
    return pProxy;
}

QModelIndex GalleryListModel::GetCurrentItem(int row) const {
    return pProxy->index(row, 0);
}

QModelIndex GalleryListModel::GetBackItem(int row) const {
    return pProxy->index(row - 1, 0);
}

QModelIndex GalleryListModel::GetNextItem(int row) const {
    return pProxy->index(row + 1, 0);
}

void GalleryListModel::ChangeItemsSize(int size) {
    if(this->size != size) {
        this->size = size;

        beginRemoveRows(QModelIndex(), 0, items.count() - 1);
        for(int i = 0; i < items.count(); i++) {
            delete items.at(i);
        }
        items.clear();
        endRemoveRows();

        QTimer::singleShot(100, this, SLOT(loadEvent()));
    }
}

Qt::ItemFlags GalleryListModel::flags(const QModelIndex& index) const {
        return QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

int GalleryListModel::rowCount(const QModelIndex&) const {
    return items.count();
}

QVariant GalleryListModel::data(const QModelIndex& index, int role) const {
    switch(role) {
    case Qt::DisplayRole:
        return QVariant(items[index.row()]->Name());

    case Qt::DecorationRole:
        return QVariant(items[index.row()]->Pixmap());

    case Qt::UserRole:
        return qVariantFromValue(items[index.row()]);

    case Qt::UserRole + 1:
        if(type == Config::ETypeGallery) {
            return qVariantFromValue(reinterpret_cast<GalleryItemData*>(items.at(index.row())->Item()));
        } else if(type == Config::ETypeCustomGallery) {
            return qVariantFromValue(reinterpret_cast<CustomGalleryItemData*>(items.at(index.row())->Item()));
        }
    }
    return QVariant();
}

void GalleryListModel::updCustomGalleryItemAngleEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(pGallery);
    if(g == item->getCustomGallery()) {
        delCustomGalleryItemEvent(item);
        addCustomGalleryItemEvent(item);
        pProxy->sort(0);
    }
}

void GalleryListModel::updCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(pGallery);
    if(g == item->getCustomGallery()) {
        GalleryListItem* i = GetItemByGalleryItem(item);
        if(i == NULL && item->getCustomId() == 0) {
            addCustomGalleryItemEvent(item);
            pProxy->sort(0);
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
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(pGallery);
    if(g == item->getCustomGallery()) {
        beginInsertRows(QModelIndex(), items.count(), items.count());
        items.append(new GalleryListItem(*item, size));
        endInsertRows();
    }
}

void GalleryListModel::delCustomGalleryItemEvent(CustomGalleryItemData* item)
{
    CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(pGallery);
    if(g == item->getCustomGallery()) {
        GalleryListItem* i = GetItemByGalleryItem(item);
        if(i) {
            int index = items.indexOf(i);

            beginRemoveRows(QModelIndex(), index, index);
            delete items.at(index);
            items.removeAt(index);
            endRemoveRows();
        }
    }
}

void GalleryListModel::loadEvent() {
    if(type == Config::ETypeGallery) {
        GalleryData* g = reinterpret_cast<GalleryData*>(pGallery);
        foreach(GalleryItemData* item, g->getItems()) {
            if(GetItemByGalleryItem(item) == NULL) {
                beginInsertRows(QModelIndex(), items.count(), items.count());
                items.append(new GalleryListItem(*item, size));
                endInsertRows();

                QTimer::singleShot(100, this, SLOT(loadEvent()));
                break;
            }
        }
    } else if(type == Config::ETypeCustomGallery) {
        CustomGalleryData* g = reinterpret_cast<CustomGalleryData*>(pGallery);
        foreach(CustomGalleryItemData* item, g->getItems()) {
            if(item->getCustomId() == 0 && GetItemByGalleryItem(item) == NULL) {
                beginInsertRows(QModelIndex(), items.count(), items.count());
                items.append(new GalleryListItem(*item, size));
                endInsertRows();

                QTimer::singleShot(100, this, SLOT(loadEvent()));
                break;
            }
        }
    }
    pProxy->sort(0);
}

GalleryListItem* GalleryListModel::GetItemByGalleryItem(GalleryItemData* pItem) {
    foreach(GalleryListItem* pi, items) {
        if(reinterpret_cast<GalleryItemData*>(pi->Item()) == pItem) {
            return pi;
        }
    }
    return NULL;
}

GalleryListItem* GalleryListModel::GetItemByGalleryItem(CustomGalleryItemData* pItem) {
    foreach(GalleryListItem* pi, items) {
        if(reinterpret_cast<CustomGalleryItemData*>(pi->Item()) == pItem) {
            return pi;
        }
    }
    return NULL;
}
