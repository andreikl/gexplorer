#include <QtCore/QTimer>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QPainter>
#include <QtGui/QMenu>

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"

#include "helpers/commonhelper.h"
#include "helpers/gstreamerhelper.h"

#include "ui/common/gallerylistmodel.h"

#include "ui/controls/titlecontrol.h"

#include "ui_viewerdialog.h"
#include "ui_titlecontrol.h"

#include "ui/dialogs/viewerdialog.h"

class ViewerControl: public QWidget {
public:
    ViewerControl(QWidget* pParent, const GalleryListModel& m, int row): QWidget(pParent), model(m) {
        currentRow = row;

        pVideoArea = new QWidget(this);
        pVideoArea->setVisible(false);
    }

    virtual ~ViewerControl() {
        GStreamerHelper::getInstance()->stopPlay();
    }

public:
    const GalleryListModel& Model() {
        return model;
    }

    int CurrentRow() {
        return currentRow;
    }

public:
    void OnBack() {
        const QModelIndex& i = model.GetBackItem(currentRow);
        if(i.isValid()) {
            currentRow = i.row();
            OnCurrent();
        }
    }

    void OnNext() {
        const QModelIndex& i = model.GetNextItem(currentRow);
        if(i.isValid()) {
            currentRow = i.row();
            OnCurrent();
        }
    }

    void OnCurrent() {
        GStreamerHelper::getInstance()->stopPlay();
        pVideoArea->setVisible(false);
        if(model.Type() == Config::ETypeGallery) {
            const QModelIndex& i = model.GetCurrentItem(currentRow);
            GalleryItemData* pItem = qVariantValue<GalleryItemData*>(i.data(Qt::UserRole + 1));
            const QString& path = CommonHelper::getPath(*pItem);
            if(ExtensionData::ExtensionVideo == pItem->getExtension().getType()) {
                GStreamerHelper::getInstance()->startPlay(pVideoArea, width(), height(), "file://" + path);
            } else {
                pixmap = QPixmap(path);
            }
            repaint();
        } else {
            const QModelIndex& i = model.GetCurrentItem(currentRow);
            CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(i.data(Qt::UserRole + 1));
            const QString& path = CommonHelper::getPath(*pItem);
            if(ExtensionData::ExtensionVideo == pItem->getItem().getExtension().getType()) {
                GStreamerHelper::getInstance()->startPlay(pVideoArea, width(), height(), "file://" + path);
            } else {
                pixmap = QPixmap(path);
            }
            repaint();
        }
    }

protected:
    void paintEvent(QPaintEvent* pEvent) {
        QWidget::paintEvent(pEvent);

        if(!pixmap.isNull() && !GStreamerHelper::getInstance()->isPlay()) {
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

private:
    const GalleryListModel& model;
    int currentRow;

private:
    QWidget* pVideoArea;
    QPixmap pixmap;
};

ViewerDialog::ViewerDialog(QWidget* pParent, GalleryListModel& model, int row): QDialog(pParent), pUi(new Ui::ViewerDialog) {
    isSlide = false;

    setWindowFlags(Qt::FramelessWindowHint);

    pUi->setupUi(this);

    pTitle = new TitleControl(this);
    pUi->plLayout->addWidget(pTitle);

    pViewer = new ViewerControl(this, model, row);
    pViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pUi->plLayout->addWidget(pViewer);

    QTimer::singleShot(10, this, SLOT(CurrentEvent()));

    CreateMenuAndActions();
    UpdateButtons();
}

ViewerDialog::~ViewerDialog() {
    delete pUi;
}

void ViewerDialog::contextMenuEvent(QContextMenuEvent* pEvent) {
    pmMenu->exec(pEvent->globalPos());
}

void ViewerDialog::BackEvent() {
    pViewer->OnBack();
    UpdateButtons();
}

void ViewerDialog::NextEvent() {
    pViewer->OnNext();
    UpdateButtons();
}

void ViewerDialog::CurrentEvent() {
    pViewer->OnCurrent();
}

void ViewerDialog::TickEvent() {
    const QModelIndex& i2 = pViewer->Model().GetNextItem(pViewer->CurrentRow());
    if(isSlide && i2.isValid()) {
        NextEvent();

        QTimer::singleShot(3000, this, SLOT(TickEvent()));
    } else {
        paSlide->setText("&Start slide show");
        paSlide->setStatusTip("Start slide show");
        paSlide->setIcon(QIcon(":/res/resources/play.png"));
        isSlide = false;
    }
}

void ViewerDialog::SlideEvent() {
    if(!isSlide) {
        paSlide->setText("&Stop slide show");
        paSlide->setStatusTip("Stop slide show");
        paSlide->setIcon(QIcon(":/res/resources/stop.png"));
        isSlide = true;

        QTimer::singleShot(3000, this, SLOT(TickEvent()));
    } else {
        paSlide->setText("&Start slide show");
        paSlide->setStatusTip("Start slide show");
        paSlide->setIcon(QIcon(":/res/resources/play.png"));
        isSlide = false;
    }
}

void ViewerDialog::CloseEvent() {
    close();
    delete this;
}

void ViewerDialog::CreateMenuAndActions() {
    pTitle->ToolBar()->setVisible(true);
    pmMenu = new QMenu(this);

    paBack = new QAction("&Previus", this);
    paBack->setStatusTip("Show previus item");
    paBack->setIcon(QIcon(":/res/resources/back.png"));
    pTitle->ToolBar()->addAction(paBack);
    pmMenu->addAction(paBack);
    connect(paBack, SIGNAL(triggered()), this, SLOT(BackEvent()));

    paNext = new QAction("&Next", this);
    paNext->setStatusTip("Show next item");
    paNext->setIcon(QIcon(":/res/resources/next.png"));
    pTitle->ToolBar()->addAction(paNext);
    pmMenu->addAction(paNext);
    connect(paNext, SIGNAL(triggered()), this, SLOT(NextEvent()));

    paSlide = new QAction("&Start slide show", this);
    paSlide->setStatusTip("Start slide show");
    paSlide->setIcon(QIcon(":/res/resources/play.png"));
    pTitle->ToolBar()->addAction(paSlide);
    pmMenu->addAction(paSlide);
    connect(paSlide, SIGNAL(triggered()), this, SLOT(SlideEvent()));

    connect(pTitle->CloseAction(), SIGNAL(triggered()), this, SLOT(CloseEvent()));
}

void ViewerDialog::UpdateButtons() {
    const QModelIndex& i1 = pViewer->Model().GetBackItem(pViewer->CurrentRow());
    if(i1.isValid()) {
        paBack->setEnabled(true);
    } else {
        paBack->setEnabled(false);
    }
    const QModelIndex& i2 = pViewer->Model().GetNextItem(pViewer->CurrentRow());
    if(i2.isValid()) {
        paNext->setEnabled(true);
        paSlide->setEnabled(true);
    } else {
        paNext->setEnabled(false);
        paSlide->setEnabled(false);
    }
}
