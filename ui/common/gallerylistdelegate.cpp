#define BORDER_SIZE 20
#define IMAGE_MARGIN 10

#include <QtCore/QDebug>

#include <QtGui/QPainter>

#include "data/customgalleryitemdata.h"

#include "ui/common/gallerylistitem.h"

#include "ui/common/gallerylistdelegate.h"

GalleryListDelegate::GalleryListDelegate(QListView* pParent): QStyledItemDelegate() {
    pList = pParent;
}

GalleryListDelegate::~GalleryListDelegate() {
}

QSize GalleryListDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex& index) const {
    GalleryListItem* pItem = qVariantValue<GalleryListItem*>(index.data(Qt::UserRole));
    int w = pList->viewport()->size().width() - 1;
    int c = w / (pItem->Pixmap().width() + BORDER_SIZE);
    int width = (c < 1)? pItem->Pixmap().width() + BORDER_SIZE: w / c;
    return QSize(width, pItem->Pixmap().height() + BORDER_SIZE);
}

void GalleryListDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(pPainter, option, index);

    GalleryListItem* pItem = qVariantValue<GalleryListItem*>(index.data(Qt::UserRole));
    if(pItem->Type() == Config::ETypeCustomGalleryItem) {
        CustomGalleryItemData* pi = reinterpret_cast<CustomGalleryItemData*>(pItem->Item());

        pPainter->setPen(QColor::fromRgb(0, 0, 0));
        if(index == dragIndex) {
            if(isDragBefore) {
                pPainter->drawRect(option.rect.left(), option.rect.top(), 1, option.rect.height());
            } else {
                pPainter->drawRect(option.rect.right() - 1, option.rect.top(), 1, option.rect.height());
            }
        }
        if(pi->getChildren().count() > 0) {
            int x = option.rect.left() + IMAGE_MARGIN + (option.rect.width() - pItem->Pixmap().width()) / 2;
            int y = option.rect.top() + pItem->Pixmap().height() - IMAGE_MARGIN;
            pPainter->drawRect(x, y - 10, 10, 10);
            pPainter->drawRect(x + 3, y - 10 - 3, 10, 10);
        }
    }
}
