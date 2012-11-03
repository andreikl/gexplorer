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

GalleryTreeModel::GalleryTreeModel(Config::ItemTypeEnum type, QObject* pParent): QAbstractItemModel(pParent) {
    this->type = type;

    aceptedGalleryIcon = QPixmap(":/res/resources/gallery_accept.png").scaled(16, 16);
    normalGalleryIcon = QPixmap(":/res/resources/gallery.png").scaled(16, 16);
    folderGalleryIcon = QPixmap(":/res/resources/folder.png").scaled(16, 16);

    pRoot = new GalleryTreeItem(NULL, "", NULL);

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
        connect(CustomGalleryHandler::getInstance(), SIGNAL(onUpdCustomGallery(CustomGalleryData*)), SLOT(updCustomGalleryEvent(CustomGalleryData*)));
        foreach(CustomGalleryData* gallery, CustomGalleryHandler::getInstance()->getCustomGalleries()) {
            addCustomGalleryEvent(gallery);
        }
    } else if(type == Config::ETypeKey) {
        connect(KeyHandler::getInstance(), SIGNAL(onAddKey(KeyData*)), SLOT(addKeyEvent(KeyData*)));
        connect(KeyHandler::getInstance(), SIGNAL(onDelKey(KeyData*)), SLOT(delKeyEvent(KeyData*)));
        connect(KeyHandler::getInstance(), SIGNAL(onUpdKey(KeyData*)), SLOT(updKeyEvent(KeyData*)));
        foreach(KeyData* key, KeyHandler::getInstance()->getKeys()) {
            addKeyEvent(key);
        }
    }
}

GalleryTreeModel::~GalleryTreeModel() {
    if(pRoot) {
        delete pRoot;
        pRoot = NULL;
    }
}

QModelIndex GalleryTreeModel::GetIndex(GalleryData* pGallery) {
    return SearchRecursive(QModelIndex(), pRoot, pGallery);
}

QModelIndex GalleryTreeModel::GetIndex(CustomGalleryData* pGallery) {
    return SearchRecursive(QModelIndex(), pRoot, pGallery);
}

QModelIndex GalleryTreeModel::GetIndex(KeyData* pKey) {
    return SearchRecursive(QModelIndex(), pRoot, pKey);
}

QModelIndex GalleryTreeModel::GetIndex(KeyData* pKey, CustomGalleryData* pGallery) {
    return SearchRecursive(QModelIndex(), pRoot, pKey, pGallery);
}

Qt::ItemFlags GalleryTreeModel::flags(const QModelIndex& index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QVariant GalleryTreeModel::headerData(int, Qt::Orientation, int) const {
    return QVariant("None");
}

int GalleryTreeModel::columnCount(const QModelIndex&) const {
    return 1;
}

int GalleryTreeModel::rowCount(const QModelIndex& parent) const {
    /*if(parent.column() > 0) {
        return 0;
    }*/
    if(!parent.isValid()) {
        return pRoot->Childs().count();
    } else {
        return reinterpret_cast<GalleryTreeItem*>(parent.internalPointer())->Childs().count();
    }
}

QModelIndex GalleryTreeModel::parent(const QModelIndex& index) const {
    /*if(!index.isValid())
        return QModelIndex();*/

    GalleryTreeItem* pParent = reinterpret_cast<GalleryTreeItem*>(index.internalPointer())->Parent();

    if(pParent == pRoot) {
        return QModelIndex();
    }

    int row = 0;
    if(pParent) {
        row = pParent->Parent()->Childs().indexOf(pParent);
    }

    return createIndex(row, 0, pParent);
}

QModelIndex GalleryTreeModel::index(int row, int column, const QModelIndex& parent) const {
    /*if(!hasIndex(row, column, parent)) {
        return QModelIndex();
    }*/

    GalleryTreeItem* pItem;
    if(!parent.isValid()) {
        pItem = pRoot->Childs().value(row);
    } else {
        pItem = reinterpret_cast<GalleryTreeItem*>(parent.internalPointer())->Childs().value(row);
    }

    if(pItem) {
        return createIndex(row, column, pItem);
    }

    return QModelIndex();
}

QVariant GalleryTreeModel::data(const QModelIndex& index, int role) const {
    GalleryTreeItem* pItem = reinterpret_cast<GalleryTreeItem*>(index.internalPointer());

    switch(role) {
    case Qt::DisplayRole:
        return QVariant(pItem->Name());

    case Qt::DecorationRole:
        if(type == Config::ETypeGallery) {
            if(pItem->Data()) {
                GalleryData* pGallery = reinterpret_cast<GalleryData*>(pItem->Data());
                if(pGallery->isAllContains()) {
                    return QVariant(aceptedGalleryIcon);
                } else {
                    return QVariant(normalGalleryIcon);
                }
            } else {
                return QVariant(folderGalleryIcon);
            }
        } else if(type == Config::ETypeCustomGallery) {
            if(pItem->Data()) {
                CustomGalleryData* pGallery = reinterpret_cast<CustomGalleryData*>(pItem->Data());
                if(pGallery->getKeys().count() > 0) {
                    return QVariant(aceptedGalleryIcon);
                } else {
                    return QVariant(normalGalleryIcon);
                }
            } else {
                return QVariant(folderGalleryIcon);
            }
        } else if(type == Config::ETypeKey) {
            if(dynamic_cast<CustomGalleryData*>(pItem->Data())) {
                return QVariant(normalGalleryIcon);
            } else {
                return QVariant(folderGalleryIcon);
            }
        }
        break;

    case Qt::UserRole:
        return qVariantFromValue(pItem);
    }
    return QVariant();
}

void GalleryTreeModel::addCustomGalleryEvent(CustomGalleryData* gallery)
{
    QString galleryName = gallery->getName();
    QStringList names = galleryName.split('/', QString::KeepEmptyParts);

    AddDataRecursive(QModelIndex(), pRoot, gallery, galleryName, names, 0);
}

void GalleryTreeModel::addGalleryEvent(GalleryData* gallery)
{
    QString galleryName = gallery->getSource();
    QStringList names = galleryName.split('/', QString::KeepEmptyParts);

    AddDataRecursive(QModelIndex(), pRoot, gallery, galleryName, names, 0);

    emit OnAddGallery(this, gallery);
}

void GalleryTreeModel::addKeyEvent(KeyData* key)
{
    QString keyName = key->getName();
    if(!keyName.endsWith('/')) {
        keyName += '/';
    }
    QStringList names = keyName.split('/', QString::KeepEmptyParts);

    QString galleryName = keyName;
    AddDataRecursive(QModelIndex(), pRoot, key, galleryName, names, 0);

    foreach(CustomGalleryData* gallery, key->getGalleries()) {
        galleryName = keyName + gallery->getName();
        names = galleryName.split('/', QString::KeepEmptyParts);

        AddDataRecursive(QModelIndex(), pRoot, gallery, galleryName, names, 0);
    }
}

void GalleryTreeModel::delCustomGalleryEvent(CustomGalleryData* gallery)
{
    SearchResult res = SearchRecursive(QModelIndex(), QModelIndex(), pRoot, gallery);
    DelData(res);
}

void GalleryTreeModel::delGalleryEvent(GalleryData* gallery)
{
    SearchResult res = SearchRecursive(QModelIndex(), QModelIndex(), pRoot, gallery);
    DelData(res);
}

void GalleryTreeModel::delKeyEvent(KeyData* key)
{
    SearchResult res = SearchRecursive(QModelIndex(), QModelIndex(), pRoot, key);
    DelData(res);

    foreach(KeyData* k, res.keys) {
        addKeyEvent(k);
    }
}

void GalleryTreeModel::updCustomGalleryEvent(CustomGalleryData* gallery)
{
    SearchResult res = SearchRecursive(QModelIndex(), QModelIndex(), pRoot, gallery);
    DelData(res);

    addCustomGalleryEvent(gallery);
}

void GalleryTreeModel::updGalleryEvent(GalleryData* gallery)
{
    SearchResult res = SearchRecursive(QModelIndex(), QModelIndex(), pRoot, gallery);
    UpdData(res);
}

void GalleryTreeModel::updKeyEvent(KeyData* key)
{
    SearchResult res = SearchRecursive(QModelIndex(), QModelIndex(), pRoot, key);
    DelData(res);

    addKeyEvent(key);
    foreach(KeyData* k, res.keys) {
        addKeyEvent(k);
    }
}

void GalleryTreeModel::AddDataRecursive(const QModelIndex& parent, GalleryTreeItem* pParent, BaseData* pData, QString& dataName, const QStringList& names, int n) {
    int pos = 0;
    bool isFind = false;
    for(int i = 0; i < pParent->Childs().count(); i++) {
        GalleryTreeItem* pItem = pParent->Childs().at(i);
        QString name = pItem->Name();

        if(name.length() > names[n].length() && name.startsWith(names[n], Qt::CaseInsensitive) && name.at(names[n].length()) == '/') {
            isFind = true;

            name = name.right(name.length() - names[n].length() - 1); n++;
            while(name.length() > names[n].length() && name.startsWith(names[n], Qt::CaseInsensitive) && name.at(names[n].length()) == '/') {
                name = name.right(name.length() - names[n].length() - 1); n++;
            }

            dataName = dataName.right(dataName.length() - (pItem->Name().length() - name.length()));
            if(name.isEmpty()) {
                AddDataRecursive(index(i, 0, parent), pItem, pData, dataName, names, n);
                break;
            } else {
                QString newName = pItem->Name().left(pItem->Name().length() - name.length());
                GalleryTreeItem* pNewItem = new GalleryTreeItem(NULL, newName, pParent);

                int j = pParent->Childs().indexOf(pItem);
                beginRemoveRows(parent, j, j);
                GalleryTreeItem* pNewItem1 = pParent->Childs().takeAt(j);
                endRemoveRows();

                pNewItem1->Name(name);
                pNewItem1->Parent(pNewItem);
                GalleryTreeItem* pNewItem2 = new GalleryTreeItem(pData, dataName, pNewItem);

                if(pNewItem1->Name().compare(pNewItem2->Name(), Qt::CaseInsensitive) < 0) {
                    pNewItem->Childs().append(pNewItem1);
                    pNewItem->Childs().append(pNewItem2);
                } else {
                    pNewItem->Childs().append(pNewItem2);
                    pNewItem->Childs().append(pNewItem1);
                }

                beginInsertRows(parent, j, j);
                pParent->Childs().insert(j, pNewItem);
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
        pParent->Childs().insert(pos, new GalleryTreeItem(pData, dataName, pParent));
        endInsertRows();
    }
}

GalleryTreeModel::SearchResult GalleryTreeModel::SearchRecursive(const QModelIndex& parent, const QModelIndex& child, GalleryTreeItem* pParent, BaseData* pData) {
    QList<KeyData*> keys;
    for(int i = 0; i < pParent->Childs().count(); i++) {
        GalleryTreeItem* pItem = pParent->Childs().at(i);
        if(pItem->Data() == pData) {
            if(type == Config::ETypeKey) {
                SearchKeysRecursive(pItem, keys);
            }
            SearchResult res = SearchResult(pItem, parent, child, keys);
            return res;
        }
        SearchResult r = SearchRecursive(child, index(i, 0, child), pItem, pData);
        if(r.pItem) {
            return r;
        }
    }
    return SearchResult(NULL, parent, parent, keys);
}

QModelIndex GalleryTreeModel::SearchRecursive(const QModelIndex& parent, GalleryTreeItem* pParent, BaseData* pKey, BaseData* pGallery) {
    if(pParent->Data() == pKey) {
        return SearchRecursive(parent, pParent, pGallery);
    }
    for(int i = 0; i < pParent->Childs().count(); i++) {
        GalleryTreeItem* pItem = pParent->Childs().at(i);
        const QModelIndex& ind = SearchRecursive(index(i, 0, parent), pItem, pKey, pGallery);
        if(ind.isValid()) {
            return ind;
        }
    }
    return QModelIndex();
}

QModelIndex GalleryTreeModel::SearchRecursive(const QModelIndex& parent, GalleryTreeItem* pParent, BaseData* pData) {
    if(pParent->Data() == pData) {
        return parent;
    }
    for(int i = 0; i < pParent->Childs().count(); i++) {
        GalleryTreeItem* pItem = pParent->Childs().at(i);
        const QModelIndex& ind = SearchRecursive(index(i, 0, parent), pItem, pData);
        if(ind.isValid()) {
            return ind;
        }
    }
    return QModelIndex();
}

void GalleryTreeModel::SearchKeysRecursive(GalleryTreeItem* pParent, QList<KeyData*>& keys) {
    for(int i = 0; i < pParent->Childs().count(); i++) {
        GalleryTreeItem* pItem = pParent->Childs().at(i);
        if(pItem->Key()) {
            keys.append(pItem->Key());
        }
        SearchKeysRecursive(pItem, keys);
    }
}

void GalleryTreeModel::DelData(SearchResult& value) {
    GalleryTreeItem* pParent = value.pItem->Parent();
    if(pParent->Childs().count() == 2 &&  pParent->Parent()) {
        int j = pParent->Parent()->Childs().indexOf(pParent);
        //qDebug() << reinterpret_cast<GalleryTreeItem*>(r.cindex.internalPointer())->Name();

        beginRemoveRows(value.pindex, j, j);
        pParent->Parent()->Childs().takeAt(j);
        endRemoveRows();

        delete pParent->Childs().takeAt(pParent->Childs().indexOf(value.pItem));
        GalleryTreeItem* pp = pParent->Childs().takeAt(0);

        pParent->Name(pParent->Name() + pp->Name());
        pParent->Data(pp->Data());
        while(pp->Childs().count()) {
            GalleryTreeItem* pc = pp->Childs().takeAt(0);
            pParent->Childs().append(pc);
            pc->Parent(pParent);
        }
        delete pp;

        beginInsertRows(value.pindex, j, j);
        pParent->Parent()->Childs().insert(j, pParent);
        endInsertRows();

    } else {
        int j = pParent->Childs().indexOf(value.pItem);
        //qDebug() << reinterpret_cast<GalleryTreeItem*>(r.cindex.internalPointer())->Name();

        beginRemoveRows(value.cindex, j, j);
        delete pParent->Childs().takeAt(j);
        endRemoveRows();
    }
}

void GalleryTreeModel::UpdData(SearchResult& value) {
    GalleryTreeItem* pParent = value.pItem->Parent();

    int j = pParent->Childs().indexOf(value.pItem);

    beginRemoveRows(value.cindex, j, j);
    GalleryTreeItem* pItem = pParent->Childs().takeAt(j);
    endRemoveRows();

    beginInsertRows(value.cindex, j, j);
    pParent->Childs().insert(j, pItem);
    endInsertRows();
}
