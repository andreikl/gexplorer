#define IMAGE_MARGIN 10
#define BORDER_SIZE 20

#include <QtCore/QVariant>
#include <QtCore/QDebug>

#include <QtGui/QPainter>

#include <QtWidgets/QScrollBar>
#include <QtWidgets/QListView>

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"

#include "ui/common/gallerylistitem.h"

#include "ui/common/gallerylistdelegate.h"

GalleryListDelegate::GalleryListDelegate(QListView* parent): QStyledItemDelegate()
{
    list = parent;
}

GalleryListDelegate::~GalleryListDelegate()
{
}

void GalleryListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    GalleryListItem* item = qvariant_cast<GalleryListItem*>(index.data(Qt::UserRole));
    int x = option.rect.left() + IMAGE_MARGIN + (option.rect.width() - item->getPixmap().width()) / 2;
    int y = option.rect.top() + item->getPixmap().height() - IMAGE_MARGIN;

    if(item->getType() == Config::ETypeCustomGalleryItem) {
        CustomGalleryItemData* i = reinterpret_cast<CustomGalleryItemData*>(item->getItem());

        painter->setPen(QColor::fromRgb(0, 0, 0));
        if(index == dragIndex) {
            if(isDragBefore) {
                painter->drawRect(option.rect.left(), option.rect.top(), 1, option.rect.height());
            } else {
                painter->drawRect(option.rect.right() - 1, option.rect.top(), 1, option.rect.height());
            }
        }
        if(i->getChildren().count() > 0) {
            painter->setPen(QColor::fromRgb(0, 255, 255));
            painter->drawRect(x, y - 10, 10, 10);
            painter->drawRect(x + 3, y - 10 - 3, 10, 10);

            if(!getReference(i)) {
                painter->setPen(QColor::fromRgb(255, 0, 0));
                painter->drawText(x + 15, y, "!");
            }
        }
    } else if(item->getType() == Config::ETypeGalleryItem) {
        GalleryItemData* i = reinterpret_cast<GalleryItemData*>(item->getItem());

        if(i->getReference() != NULL) {
            painter->setPen(QColor::fromRgb(0, 255, 0));
            painter->drawText(x + 15, y, "!");
        }

        if(i->getCustomItems().count() > 0) {
            painter->setPen(QColor::fromRgb(255, 0, 255));
            painter->drawText(x + 25, y, "!!!");
        }
    }
}

QSize GalleryListDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex& index) const
{
    GalleryListItem* item = qvariant_cast<GalleryListItem*>(index.data(Qt::UserRole));
    int w = list->width() - list->verticalScrollBar()->width() - 3;
    int c = w / (item->getSize() + BORDER_SIZE);
    int width = (c < 1)? item->getSize() + BORDER_SIZE: w / c;

    return QSize(width, item->getSize() + BORDER_SIZE);
}

const QModelIndex& GalleryListDelegate::getDragIndex() const
{
    return dragIndex;
}

bool GalleryListDelegate::getIsDragBefore() const
{
    return isDragBefore;
}

void GalleryListDelegate::setDragIndex(const QModelIndex& index, bool isBefore)
{
    lastDragIndex = dragIndex;
    dragIndex = index;
    lastIsDragBefore = isDragBefore;
    isDragBefore = isBefore;

    if(dragIndex.isValid()) {
        list->update(dragIndex);
    }
    if(lastDragIndex.isValid()) {
        list->update(lastDragIndex);
    }
}

bool GalleryListDelegate::getReference(CustomGalleryItemData* item) const
{
    bool isRef = true;
    foreach(CustomGalleryItemData* gi, item->getChildren()) {
        if(!getReference(gi) || gi->getItem().getReference() == NULL) {
            isRef = false;
            break;
        }
    }
    return isRef;
}
