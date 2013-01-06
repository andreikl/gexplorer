#include <QtCore/QDebug>

#include <QtGui/QPainter>

#include <QtWidgets/QVBoxLayout>

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"

#include "helpers/commonhelper.h"

#include "ui/common/gallerylistmodel.h"

#include "ui/controls/videocontrol.h"

#include "ui/controls/viewercontrol.h"

ViewerControl::ViewerControl(QWidget* parent, const GalleryListModel& m, int row): QWidget(parent), model(m), currentRow(row), video(NULL)
{
    setLayout(new QVBoxLayout(this));
}

ViewerControl::~ViewerControl()
{
}

const GalleryListModel& ViewerControl::getModel()
{
    return model;
}

int ViewerControl::getCurrentRow()
{
    return currentRow;
}

void ViewerControl::currentEvent()
{
    ExtensionData::ExtensionTypeEnum type;
    QString path;
    int angle;

    int w;
    int h;

    const QModelIndex& i = model.getCurrentItem(currentRow);
    if(model.getType() == Config::ETypeGallery) {
        GalleryItemData* item = qvariant_cast<GalleryItemData*>(i.data(Qt::UserRole + 1));
        type = item->getExtension().getType();
        path = CommonHelper::getPath(*item);
        angle = CommonHelper::getAngle(*item);
        w = (item->width != 0)? item->width: 800;
        h = (item->height != 0)? item->height: 600;
    } else {
        CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(i.data(Qt::UserRole + 1));
        type = item->getItem().getExtension().getType();
        path = CommonHelper::getPath(*item);
        angle = CommonHelper::getAngle(*item);
        w = (item->width != 0)? item->width: 800;
        h = (item->height != 0)? item->height: 600;
    }

    if(ExtensionData::ExtensionVideo == type) {
        if(!video) {
            video = new VideoControl(this);
            layout()->addWidget(video);
            video->show();
        }
        video->startPlay(path, w, h);
    } else if(ExtensionData::ExtensionImage == type) {
        delete video;
        video = NULL;
        pixmap = QPixmap(path);
        if(angle != 0) {
            QMatrix rm;
            rm.rotate(angle);
            pixmap = pixmap.transformed(rm);
        }
        repaint();
    }
}

void ViewerControl::backEvent()
{
    const QModelIndex& i = model.getBackItem(currentRow);
    if(i.isValid()) {
        currentRow = i.row();
        currentEvent();
    }
}

void ViewerControl::nextEvent()
{
    const QModelIndex& i = model.getNextItem(currentRow);
    if(i.isValid()) {
        currentRow = i.row();
        currentEvent();
    }
}

void ViewerControl::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    if(!pixmap.isNull() && !video) {
        //qDebug("%d %d", width(), height());

        QPainter paint(this);
        if(width() < pixmap.width() || height() < pixmap.height()) {
            float tw = pixmap.width() / (float)width();
            float th = pixmap.height() / (float)height();
            if(tw > th) {
                const QPixmap& t = pixmap.scaledToWidth(width());
                int x = (width() - t.width()) / 2;
                int y = (height() - t.height()) / 2;
                paint.drawPixmap(x, y, t);
            } else {
                const QPixmap& t = pixmap.scaledToHeight(height());
                int x = (width() - t.width()) / 2;
                int y = (height() - t.height()) / 2;
                paint.drawPixmap(x, y, t);
            }
        } else {
            int x = (width() - pixmap.width()) / 2;
            int y = (height() - pixmap.height()) / 2;
            paint.drawPixmap(x, y, pixmap);
        }
    }
}
