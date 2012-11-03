#ifndef GALLERYTREEMODEL_H
#define GALLERYTREEMODEL_H

#include <QtCore/QAbstractItemModel>

#include <QtGui/QPixmap>

#include "config.h"

class CustomGalleryData;
class GalleryTreeItem;
class GalleryData;
class BaseData;
class KeyData;

class GalleryTreeModel: public QAbstractItemModel {
    Q_OBJECT

    struct SearchResult {
    public:
        SearchResult(GalleryTreeItem* pItem, const QModelIndex& p, const QModelIndex& c, const QList<KeyData*>& keys): pindex(p), cindex(c) {
            this->pItem = pItem;
            this->keys = keys;
        }

    public:
        GalleryTreeItem* pItem;
        QModelIndex pindex;
        QModelIndex cindex;

    public:
        QList<KeyData*> keys;
    };

public:
    GalleryTreeModel(Config::ItemTypeEnum type, QObject* pParent = NULL);
    virtual ~GalleryTreeModel();

public:
    QModelIndex GetIndex(GalleryData* pGallery);
    QModelIndex GetIndex(CustomGalleryData* pGallery);
    QModelIndex GetIndex(KeyData* pKey);
    QModelIndex GetIndex(KeyData* pKey, CustomGalleryData* pGallery);

// QAbstractItemModel
public:
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

signals:
    void OnAddGallery(GalleryTreeModel* pSender, GalleryData* pValue);

private slots:
    void addCustomGalleryEvent(CustomGalleryData* gallery);
    void addGalleryEvent(GalleryData* gallery);
    void addKeyEvent(KeyData* key);

private slots:
    void delCustomGalleryEvent(CustomGalleryData* gallery);
    void delGalleryEvent(GalleryData* gallery);
    void delKeyEvent(KeyData* key);

private slots:
    void updCustomGalleryEvent(CustomGalleryData* gallery);
    void updGalleryEvent(GalleryData* gallery);
    void updKeyEvent(KeyData* key);

private:
    void AddDataRecursive(const QModelIndex& parent, GalleryTreeItem* pParent, BaseData* pData, QString& dataName, const QStringList& names, int n);
    SearchResult SearchRecursive(const QModelIndex& parent, const QModelIndex& child, GalleryTreeItem* pParent, BaseData* pData);
    QModelIndex SearchRecursive(const QModelIndex& parent, GalleryTreeItem* pParent, BaseData* pKey, BaseData* pGallery);
    QModelIndex SearchRecursive(const QModelIndex& parent, GalleryTreeItem* pParent, BaseData* pData);
    void SearchKeysRecursive(GalleryTreeItem* pParent, QList<KeyData*>& keys);

private:
    void DelData(SearchResult& value);
    void UpdData(SearchResult& value);

private:
    Config::ItemTypeEnum type;
    GalleryTreeItem* pRoot;

private:
    QPixmap aceptedGalleryIcon;
    QPixmap normalGalleryIcon;
    QPixmap folderGalleryIcon;
};

#endif // GALLERYTREEMODEL_H
