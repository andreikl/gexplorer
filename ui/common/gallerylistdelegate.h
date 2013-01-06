#ifndef GALLERYLISTDELEGATE_H
#define GALLERYLISTDELEGATE_H

#include <QtCore/QModelIndex>

#include <QtWidgets/QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QListView;
QT_END_NAMESPACE

class CustomGalleryItemData;

class GalleryListDelegate: public QStyledItemDelegate
{
public:
    GalleryListDelegate(QListView* parent);
    virtual ~GalleryListDelegate();

public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
    const QModelIndex& getDragIndex() const;
    bool getIsDragBefore() const;

public:
    void setDragIndex(const QModelIndex& index, bool isBefore);

private:
    bool getReference(CustomGalleryItemData* item) const;

private:
    QModelIndex lastDragIndex;
    QModelIndex dragIndex;

private:
    bool isDragBefore;
    bool lastIsDragBefore;

private:
    QListView* list;
};

#endif // GALLERYLISTDELEGATE_H
