#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include "data/customgallerydata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "helpers/commonhelper.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"
#include "handlers/keyhandler.h"

#include "ui/common/gallerytreeitem.h"

#include "ui/common/gallerytreemodel.h"

GalleryTreeModel::SearchResult::SearchResult(GalleryTreeItem* item, const QModelIndex& p, const QModelIndex& c): pindex(p), cindex(c)
{
    this->item = item;
}

GalleryTreeModel::GalleryTreeModel(Config::ItemTypeEnum type, QObject* parent): QAbstractItemModel(parent)
{
    this->type = type;

    aceptedGalleryIcon = QPixmap(":/res/resources/gallery_accept.png").scaled(16, 16);
    deletedGalleryIcon = QPixmap(":/res/resources/gallery_delete.png").scaled(16, 16);
    normalGalleryIcon = QPixmap(":/res/resources/gallery.png").scaled(16, 16);
    folderGalleryIcon = QPixmap(":/res/resources/folder.png").scaled(16, 16);

    root = new GalleryTreeItem(NULL, "", NULL);

    if(type == Config::ETypeGallery) {
        connect(GalleryHandler::getInstance(), SIGNAL(onAddGallery(GalleryData*)), SLOT(addGalleryEvent(GalleryData*)));
        connect(GalleryHandler::getInstance(), SIGNAL(onDelGallery(GalleryData*)), SLOT(delGalleryEvent(GalleryData*)));
        connect(GalleryHandler::getInstance(), SIGNAL(onUpdGallery(GalleryData*)), SLOT(updGalleryEvent(GalleryData*)));
        foreach(GalleryData* gallery, GalleryHandler::getInstance()->getGalleries()) {
            addGalleryEvent(gallery);
        }
    } else if(type == Config::ETypeCustomGallery) {
        connect(CustomGalleryHandler::getInstance(), SIGNAL(onAddCustomGallery(CustomGalleryData*)), SLOT(addCustomGalleryEvent(CustomGalleryData*)));
        connect(CustomGalleryHandler::getInstance(), SIGNAL(onDelCustomGallery(CustomGalleryData*)), SLOT(delCustomGalleryEvent(CustomGalleryData*)));
        connect(CustomGalleryHandler::getInstance(), SIGNAL(onUpdCustomGallery(CustomGalleryData*, const char*)), SLOT(updCustomGalleryEvent(CustomGalleryData*, const char*)));
        foreach(CustomGalleryData* gallery, CustomGalleryHandler::getInstance()->getCustomGalleries()) {
            addCustomGalleryEvent(gallery);
        }
    } else if(type == Config::ETypeKey) {
        connect(KeyHandler::getInstance(), SIGNAL(onAddToKey(KeyData*, CustomGalleryData*)), SLOT(addToKeyEvent(KeyData*, CustomGalleryData*)));
        connect(KeyHandler::getInstance(), SIGNAL(onAddKey(KeyData*)), SLOT(addKeyEvent(KeyData*)));
        connect(KeyHandler::getInstance(), SIGNAL(onDelFromKey(KeyData*, CustomGalleryData*)), SLOT(delFromKeyEvent(KeyData*, CustomGalleryData*)));
        connect(KeyHandler::getInstance(), SIGNAL(onDelKey(KeyData*)), SLOT(delKeyEvent(KeyData*)));
        connect(KeyHandler::getInstance(), SIGNAL(onUpdKey(KeyData*)), SLOT(updKeyEvent(KeyData*)));
        foreach(KeyData* key, KeyHandler::getInstance()->getKeys()) {
            addKeyEvent(key);
        }
    }
}

GalleryTreeModel::~GalleryTreeModel()
{
    if(root) {
        delete root;
        root = NULL;
    }
}

QModelIndex GalleryTreeModel::getIndex(KeyData* key, CustomGalleryData* gallery)
{
    return searchRecursive(QModelIndex(), root, key, gallery);
}

QModelIndex GalleryTreeModel::getIndex(CustomGalleryData* gallery)
{
    return searchRecursive(QModelIndex(), root, gallery);
}

QModelIndex GalleryTreeModel::getIndex(GalleryData* gallery)
{
    return searchRecursive(QModelIndex(), root, gallery);
}

QModelIndex GalleryTreeModel::getIndex(KeyData* key)
{
    return searchRecursive(QModelIndex(), root, key);
}

QVariant GalleryTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    (void) section;
    (void) orientation;
    (void) role;
    return QVariant("None");
}

QVariant GalleryTreeModel::data(const QModelIndex& index, int role) const
{
    GalleryTreeItem* item = reinterpret_cast<GalleryTreeItem*>(index.internalPointer());

    switch(role) {
    case Qt::DisplayRole:
        return QVariant(item->getName());

    case Qt::DecorationRole:
        if(type == Config::ETypeGallery) {
            if(item->getData()) {
                GalleryData* gallery = reinterpret_cast<GalleryData*>(item->getData());
                if(gallery->getType() == GalleryData::DeletedGallery) {
                    return QVariant(deletedGalleryIcon);
                } else {
                    if(gallery->isAllContains()) {
                        return QVariant(aceptedGalleryIcon);
                    } else {
                        return QVariant(normalGalleryIcon);
                    }
                }
            } else {
                return QVariant(folderGalleryIcon);
            }
        } else if(type == Config::ETypeCustomGallery) {
            if(item->getData()) {
                CustomGalleryData* gallery = reinterpret_cast<CustomGalleryData*>(item->getData());
                if(gallery->getKeys().count() > 0) {
                    return QVariant(aceptedGalleryIcon);
                } else {
                    return QVariant(normalGalleryIcon);
                }
            } else {
                return QVariant(folderGalleryIcon);
            }
        } else if(type == Config::ETypeKey) {
            if(dynamic_cast<CustomGalleryData*>(item->getData())) {
                return QVariant(normalGalleryIcon);
            } else {
                return QVariant(folderGalleryIcon);
            }
        }
        break;

    case Qt::UserRole:
        return qVariantFromValue(item);
    }
    return QVariant();
}

QModelIndex GalleryTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    GalleryTreeItem* item;
    if(!parent.isValid()) {
        item = root->getChilds().value(row);
    } else {
        item = reinterpret_cast<GalleryTreeItem*>(parent.internalPointer())->getChilds().value(row);
    }

    if(item) {
        return createIndex(row, column, item);
    }

    return QModelIndex();
}

int GalleryTreeModel::columnCount(const QModelIndex& parent) const
{
    (void)parent;
    return 1;
}

int GalleryTreeModel::rowCount(const QModelIndex& parent) const
{
    /*if(parent.column() > 0) {
        return 0;
    }*/
    if(!parent.isValid()) {
        return root->getChilds().count();
    } else {
        return reinterpret_cast<GalleryTreeItem*>(parent.internalPointer())->getChilds().count();
    }
}

Qt::ItemFlags GalleryTreeModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QModelIndex GalleryTreeModel::parent(const QModelIndex& index) const
{
    GalleryTreeItem* parent = reinterpret_cast<GalleryTreeItem*>(index.internalPointer())->getParent();

    if(parent == root) {
        return QModelIndex();
    }

    int row = 0;
    if(parent) {
        row = parent->getParent()->getChilds().indexOf(parent);
    }

    return createIndex(row, 0, parent);
}

void GalleryTreeModel::addCustomGalleryEvent(CustomGalleryData* gallery)
{
    bool filter = false;
    if(Config::getInstance()->getCustomGalleryFilterType() == Config::ESortedFilter && gallery->getKeys().count() > 0) {
        filter = true;
    } else if(Config::getInstance()->getCustomGalleryFilterType() == Config::EUnsortedFilter && gallery->getKeys().count() <= 0) {
        filter = true;
    } else if(Config::getInstance()->getCustomGalleryFilterType() == Config::EAllFilter) {
        filter = true;
    }

    if(filter) {
        QString galleryName = gallery->getName();
        QStringList names = galleryName.split('/', QString::KeepEmptyParts);

        addDataRecursive(QModelIndex(), root, gallery, galleryName, names, 0);
    }
}

void GalleryTreeModel::addGalleryEvent(GalleryData* gallery)
{
    bool filter = false;
    if(Config::getInstance()->getGalleryFilterType() == Config::ESortedFilter && gallery->isAllContains()) {
        filter = true;
    } else if(Config::getInstance()->getGalleryFilterType() == Config::EUnsortedFilter && !gallery->isAllContains()) {
        filter = true;
    } else if(Config::getInstance()->getGalleryFilterType() == Config::EAllFilter) {
        filter = true;
    }

    if(filter) {
        QString galleryName = gallery->getSource();
        QStringList names = galleryName.split('/', QString::KeepEmptyParts);

        addDataRecursive(QModelIndex(), root, gallery, galleryName, names, 0);

        emit onAddGallery(gallery);
    }
}

void GalleryTreeModel::addToKeyEvent(KeyData* key, CustomGalleryData* gallery) {
    QString keyName = key->getName();
    if(!keyName.endsWith('/')) {
        keyName += '/';
    }

    QString galleryName = keyName + gallery->getName();
    QStringList names = galleryName.split('/', QString::KeepEmptyParts);

    addDataRecursive(QModelIndex(), root, gallery, galleryName, names, 0);
}

void GalleryTreeModel::addKeyEvent(KeyData* key)
{
    QString keyName = key->getName();
    if(!keyName.endsWith('/')) {
        keyName += '/';
    }
    QStringList names = keyName.split('/', QString::KeepEmptyParts);

    QString galleryName = keyName;
    addDataRecursive(QModelIndex(), root, key, galleryName, names, 0);

    foreach(CustomGalleryData* gallery, key->getGalleries()) {
        galleryName = keyName + gallery->getName();
        names = galleryName.split('/', QString::KeepEmptyParts);

        addDataRecursive(QModelIndex(), root, gallery, galleryName, names, 0);
    }
}

void GalleryTreeModel::delCustomGalleryEvent(CustomGalleryData* gallery)
{
    SearchResult res = searchRecursive(QModelIndex(), QModelIndex(), root, gallery);
    delData(res);
}

void GalleryTreeModel::delGalleryEvent(GalleryData* gallery)
{
    SearchResult res = searchRecursive(QModelIndex(), QModelIndex(), root, gallery);
    delData(res);
}

void GalleryTreeModel::delFromKeyEvent(KeyData* key, CustomGalleryData* gallery)
{
    SearchResult resKey = searchRecursive(QModelIndex(), QModelIndex(), root, key);
    SearchResult resGallery = searchRecursive(resKey.cindex, index(resKey.item->getParent()->getChilds().indexOf(resKey.item), 0, resKey.cindex), resKey.item, gallery);
    delData(resGallery, resKey.item);
}

void GalleryTreeModel::delKeyEvent(KeyData* key)
{
    QList<KeyData*> keys;
    SearchResult res = searchRecursive(QModelIndex(), QModelIndex(), root, key);
    searchKeysRecursive(res.item, keys);

    delData(res);
    foreach(KeyData* k, keys) {
        addKeyEvent(k);
    }
}

void GalleryTreeModel::updCustomGalleryEvent(CustomGalleryData* gallery, const char* name)
{
    SearchResult res = searchRecursive(QModelIndex(), QModelIndex(), root, gallery);
    if(strcmp(name, "name") == 0) {
        delData(res);
        addCustomGalleryEvent(gallery);
    } else {
        updData(res);
    }
}

void GalleryTreeModel::updGalleryEvent(GalleryData* gallery)
{
    SearchResult res = searchRecursive(QModelIndex(), QModelIndex(), root, gallery);
    updData(res);
}

void GalleryTreeModel::updKeyEvent(KeyData* key)
{
    QList<KeyData*> keys;
    SearchResult res = searchRecursive(QModelIndex(), QModelIndex(), root, key);
    searchKeysRecursive(res.item, keys);

    delData(res);

    addKeyEvent(key);
    foreach(KeyData* k, keys) {
        addKeyEvent(k);
    }
}

void GalleryTreeModel::addDataRecursive(const QModelIndex& parent, GalleryTreeItem* gparent, BaseData* data, QString& dataName, const QStringList& names, int n)
{
    int pos = 0;
    bool isFind = false;
    for(int i = 0; i < gparent->getChilds().count(); i++) {
        GalleryTreeItem* item = gparent->getChilds().at(i);
        QString name = item->getName();

        if(name.length() > names[n].length() && name.startsWith(names[n], Qt::CaseInsensitive) && name.at(names[n].length()) == '/') {
            isFind = true;

            name = name.right(name.length() - names[n].length() - 1); n++;
            while(n < names.length() && name.length() > names[n].length() && name.startsWith(names[n], Qt::CaseInsensitive) && name.at(names[n].length()) == '/') {
                name = name.right(name.length() - names[n].length() - 1); n++;
            }

            dataName = dataName.right(dataName.length() - (item->getName().length() - name.length()));
            if(name.isEmpty()) {
                addDataRecursive(index(i, 0, parent), item, data, dataName, names, n);
                break;
            } else {
                QString newName = item->getName().left(item->getName().length() - name.length());
                GalleryTreeItem* newItem = new GalleryTreeItem(NULL, newName, gparent);

                int j = gparent->getChilds().indexOf(item);
                beginRemoveRows(parent, j, j);
                GalleryTreeItem* newItem1 = gparent->getChilds().takeAt(j);
                endRemoveRows();

                newItem1->setName(name);
                newItem1->setParent(newItem);
                GalleryTreeItem* newItem2 = new GalleryTreeItem(data, dataName, newItem);

                if(newItem1->getName().compare(newItem2->getName(), Qt::CaseInsensitive) < 0) {
                    newItem->getChilds().append(newItem1);
                    newItem->getChilds().append(newItem2);
                } else {
                    newItem->getChilds().append(newItem2);
                    newItem->getChilds().append(newItem1);
                }

                beginInsertRows(parent, j, j);
                gparent->getChilds().insert(j, newItem);
                endInsertRows();
                break;
            }
        }

        if(name.compare(names[n], Qt::CaseInsensitive) < 0) {
            pos++;
        }
    }
    if(!isFind) {
        beginInsertRows(parent, pos, pos);
        gparent->getChilds().insert(pos, new GalleryTreeItem(data, dataName, gparent));
        endInsertRows();
    }
}

GalleryTreeModel::SearchResult GalleryTreeModel::searchRecursive(const QModelIndex& parent, const QModelIndex& child, GalleryTreeItem* gparent, BaseData* data)
{
    for(int i = 0; i < gparent->getChilds().count(); i++) {
        GalleryTreeItem* item = gparent->getChilds().at(i);
        if(item->getData() == data) {
            SearchResult res = SearchResult(item, parent, child);
            return res;
        }
        SearchResult r = searchRecursive(child, index(i, 0, child), item, data);
        if(r.item) {
            return r;
        }
    }
    return SearchResult(NULL, parent, parent);
}

QModelIndex GalleryTreeModel::searchRecursive(const QModelIndex& parent, GalleryTreeItem* gparent, BaseData* key, BaseData* gallery)
{
    if(gparent->getData() == key) {
        return searchRecursive(parent, gparent, gallery);
    }
    for(int i = 0; i < gparent->getChilds().count(); i++) {
        GalleryTreeItem* item = gparent->getChilds().at(i);
        const QModelIndex& ind = searchRecursive(index(i, 0, parent), item, key, gallery);
        if(ind.isValid()) {
            return ind;
        }
    }
    return QModelIndex();
}

QModelIndex GalleryTreeModel::searchRecursive(const QModelIndex& parent, GalleryTreeItem* gparent, BaseData* data)
{
    if(gparent->getData() == data) {
        return parent;
    }
    for(int i = 0; i < gparent->getChilds().count(); i++) {
        GalleryTreeItem* item = gparent->getChilds().at(i);
        const QModelIndex& ind = searchRecursive(index(i, 0, parent), item, data);
        if(ind.isValid()) {
            return ind;
        }
    }
    return QModelIndex();
}

void GalleryTreeModel::searchKeysRecursive(GalleryTreeItem* parent, QList<KeyData*>& keys)
{
    for(int i = 0; i < parent->getChilds().count(); i++) {
        GalleryTreeItem* item = parent->getChilds().at(i);
        if(item->getKey()) {
            keys.append(item->getKey());
        }
        searchKeysRecursive(item, keys);
    }
}

void GalleryTreeModel::delData(SearchResult& value, GalleryTreeItem* parent)
{
    GalleryTreeItem* p = value.item->getParent();
    if(p->getChilds().count() == 2 &&  p->getParent() && p != parent) {
        int j = p->getParent()->getChilds().indexOf(p);
        //qDebug() << reinterpret_cast<GalleryTreeItem*>(r.cindex.internalPointer())->Name();

        beginRemoveRows(value.pindex, j, j);
        p->getParent()->getChilds().takeAt(j);
        endRemoveRows();

        delete p->getChilds().takeAt(p->getChilds().indexOf(value.item));
        GalleryTreeItem* i = p->getChilds().takeAt(0);

        p->setName(p->getName() + i->getName());
        p->setData(i->getData());
        while(i->getChilds().count()) {
            GalleryTreeItem* ic = i->getChilds().takeAt(0);
            p->getChilds().append(ic);
            ic->setParent(p);
        }
        delete i;

        beginInsertRows(value.pindex, j, j);
        p->getParent()->getChilds().insert(j, p);
        endInsertRows();

    } else {
        int j = p->getChilds().indexOf(value.item);
        //qDebug() << reinterpret_cast<GalleryTreeItem*>(value.cindex.internalPointer())->getName();

        beginRemoveRows(value.cindex, j, j);
        delete p->getChilds().takeAt(j);
        endRemoveRows();
    }
}

void GalleryTreeModel::updData(SearchResult& value)
{
    emit dataChanged(value.cindex, value.cindex);
    /*GalleryTreeItem* parent = value.item->getParent();

    int j = parent->getChilds().indexOf(value.item);

    beginRemoveRows(value.cindex, j, j);
    GalleryTreeItem* item = parent->getChilds().takeAt(j);
    endRemoveRows();

    beginInsertRows(value.cindex, j, j);
    parent->getChilds().insert(j, item);
    endInsertRows();*/
}
