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

class GalleryTreeModel: public QAbstractItemModel
{
    Q_OBJECT

    struct SearchResult
    {
    public:
        SearchResult(GalleryTreeItem* item, const QModelIndex& p, const QModelIndex& c);

    public:
        GalleryTreeItem* item;
        QModelIndex pindex;
        QModelIndex cindex;
    };


public:
    GalleryTreeModel(Config::ItemTypeEnum type, QObject* parent);
    virtual ~GalleryTreeModel();

public:
    QModelIndex getIndex(KeyData* key, CustomGalleryData* gallery);
    QModelIndex getIndex(CustomGalleryData* gallery);
    QModelIndex getIndex(GalleryData* gallery);
    QModelIndex getIndex(KeyData* key);

public:
    void reload();

// QAbstractItemModel
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QModelIndex parent(const QModelIndex& index) const;

signals:
    void onAddGallery(GalleryData* value);

private slots:
    void addCustomGalleryEvent(CustomGalleryData* gallery);
    void addGalleryEvent(GalleryData* gallery);

    void addToKeyEvent(KeyData* key, CustomGalleryData* gallery);
    void addKeyEvent(KeyData* key);

private slots:
    void delCustomGalleryEvent(CustomGalleryData* gallery);
    void delGalleryEvent(GalleryData* gallery);

    void delFromKeyEvent(KeyData* key, CustomGalleryData* gallery);
    void delKeyEvent(KeyData* key);

private slots:
    void updCustomGalleryEvent(CustomGalleryData* gallery, const char* name);
    void updKeyEvent(KeyData* key);
    void updGalleryEvent(GalleryData* gallery);

private:
    void addDataRecursive(const QModelIndex& parent, GalleryTreeItem* gparent, BaseData* data, QString& dataName, const QStringList& names, int n);
    SearchResult searchRecursive(const QModelIndex& parent, const QModelIndex& child, GalleryTreeItem* gparent, BaseData* data);
    QModelIndex searchRecursive(const QModelIndex& parent, GalleryTreeItem* gparent, BaseData* key, BaseData* gallery);
    QModelIndex searchRecursive(const QModelIndex& parent, GalleryTreeItem* gparent, BaseData* data);
    void searchKeysRecursive(GalleryTreeItem* parent, QList<KeyData*>& keys);

private:
    void delData(SearchResult& value, GalleryTreeItem* parent = NULL);
    void updData(SearchResult& value);

private:
    Config::ItemTypeEnum type;
    GalleryTreeItem* root;

private:
    QPixmap aceptedGalleryIcon;
    QPixmap deletedGalleryIcon;
    QPixmap normalGalleryIcon;
    QPixmap folderGalleryIcon;
};

#endif // GALLERYTREEMODEL_H
