#ifndef GALLERYLISTDELEGATE_H
#define GALLERYLISTDELEGATE_H

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QListView>

class GalleryListDelegate: public QStyledItemDelegate {
public:
    GalleryListDelegate(QListView* pParent);
    virtual ~GalleryListDelegate();

public:
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public:
    const QModelIndex& DragIndex() const {
        return dragIndex;
    }

    bool IsDragBefore() const {
        return isDragBefore;
    }

    void DragIndex(const QModelIndex& index, bool isBefore) {
        lastDragIndex = dragIndex;
        dragIndex = index;
        lastIsDragBefore = isDragBefore;
        isDragBefore = isBefore;

        if(dragIndex.isValid()) {
            pList->update(dragIndex);
        }
        if(lastDragIndex.isValid()) {
            pList->update(lastDragIndex);
        }
    }

private:
    QModelIndex dragIndex;
    bool isDragBefore;
    QModelIndex lastDragIndex;
    bool lastIsDragBefore;

private:
    QListView* pList;
};

#endif // GALLERYLISTDELEGATE_H
