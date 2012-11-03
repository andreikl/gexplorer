#define MIN_SCALE 0.1
#define MAX_SCALE 2.0
#define SCALE_STEP 0.2

#include <QtGui/QScrollBar>
#include <QtGui/QPainter>

#include "data/customgalleryitemdata.h"

#include "helpers/commonhelper.h"

#include "ui_unitedialog.h"

#include "ui/dialogs/unitedialog.h"

UniteDialog::UniteDialog(QWidget* pParent, CustomGalleryItemData& item1, CustomGalleryItemData& item2): QDialog(pParent), pUi(new Ui::UniteDialog) {
    setModal(true);

    pUi->setupUi(this);

    const QString& path1 = CommonHelper::getPath(item1);
    pixmap1 = QPixmap(path1);
    if(item1.getAngle() != 0) {
        QMatrix rm;
        rm.rotate(item1.getAngle());
        pixmap1 = pixmap1.transformed(rm);
    }

    const QString& path2 = CommonHelper::getPath(item2);
    pixmap2 = QPixmap(path2);
    if(item2.getAngle() != 0) {
        QMatrix rm;
        rm.rotate(item2.getAngle());
        pixmap2 = pixmap2.transformed(rm);
    }

    pUi->prImage1->setText(item1.getName() + " (" + QString::number(pixmap1.width()) + "x" + QString::number(pixmap1.height()) + ")");
    pUi->prImage2->setText(item2.getName() + " (" + QString::number(pixmap2.width()) + "x" + QString::number(pixmap2.height()) + ")");
    if(pixmap1.width() >= pixmap2.width()) {
        pUi->prImage1->setChecked(true);
        isFirst = true;
    } else {
        pUi->prImage2->setChecked(true);
        isFirst = false;
    }

    scale = 1.0;
    Scale(scale);

    connect(pUi->prImage1, SIGNAL(clicked()), SLOT(OnChecked()));
    connect(pUi->prImage2, SIGNAL(clicked()), SLOT(OnChecked()));

    connect(pUi->psImage1->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnH1Changed(int)));
    connect(pUi->psImage1->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnV1Changed(int)));

    connect(pUi->psImage2->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnH2Changed(int)));
    connect(pUi->psImage2->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(OnV2Changed(int)));

    connect(pUi->pbZoomIn, SIGNAL(clicked()), SLOT(OnZoomIn()));
    connect(pUi->pbZoomOut, SIGNAL(clicked()), SLOT(OnZoomOut()));

    connect(pUi->pbUnite, SIGNAL(clicked()), SLOT(accept()));
    connect(pUi->pbCancel, SIGNAL(clicked()), SLOT(close()));
}

UniteDialog::~UniteDialog() {
    delete pUi;
}

bool UniteDialog::IsFirst() {
    return isFirst;
}

void UniteDialog::OnH1Changed(int value) {
    if(pUi->psImage2->horizontalScrollBar()->value() != value) {
        pUi->psImage2->horizontalScrollBar()->setValue(value);
    }
}

void UniteDialog::OnV1Changed(int value) {
    if(pUi->psImage2->verticalScrollBar()->value() != value) {
        pUi->psImage2->verticalScrollBar()->setValue(value);
    }
}

void UniteDialog::OnH2Changed(int value) {
    if(pUi->psImage1->horizontalScrollBar()->value() != value) {
        pUi->psImage1->horizontalScrollBar()->setValue(value);
    }
}

void UniteDialog::OnV2Changed(int value) {
    if(pUi->psImage1->verticalScrollBar()->value() != value) {
        pUi->psImage1->verticalScrollBar()->setValue(value);
    }
}

void UniteDialog::OnChecked() {
    isFirst = (pUi->prImage1->isChecked())? true: false;
}

void UniteDialog::OnZoomOut() {
    scale -= SCALE_STEP;
    if(scale <= MIN_SCALE) {
        scale = MIN_SCALE;
        pUi->pbZoomOut->setEnabled(false);
    }
    pUi->pbZoomIn->setEnabled(true);
    Scale(scale);
}

void UniteDialog::OnZoomIn() {
    scale += SCALE_STEP;
    if(scale >= MAX_SCALE) {
        scale = MAX_SCALE;
        pUi->pbZoomIn->setEnabled(false);
    }
    pUi->pbZoomOut->setEnabled(true);
    Scale(scale);
}

void UniteDialog::Scale(double scale) {
    int w = 0;
    int h = 0;
    if(pixmap1.width() > pixmap2.width()) {
        w = pixmap1.width();
        h = pixmap1.height();
    } else {
        w = pixmap2.width();
        h = pixmap2.height();
    }
    w *= scale;
    h *= scale;
    const QPixmap& p1 = pixmap1.scaled(w, h);
    const QPixmap& p2 = pixmap2.scaled(w, h);

    pUi->plImage1->setPixmap(p1);
    pUi->plImage2->setPixmap(p2);
}
