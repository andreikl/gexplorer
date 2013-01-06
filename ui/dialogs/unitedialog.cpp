#define MIN_SCALE 0.1
#define MAX_SCALE 2.0
#define SCALE_STEP 0.2

#include <QtWidgets/QScrollBar>

#include "data/customgalleryitemdata.h"

#include "helpers/commonhelper.h"

#include "ui/dialogs/unitedialog.h"
#include "ui_unitedialog.h"

UniteDialog::UniteDialog(QWidget* parent, CustomGalleryItemData& item1, CustomGalleryItemData& item2): QDialog(parent), ui(new Ui::UniteDialog)
{
    setModal(true);

    ui->setupUi(this);

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

    ui->rImage1->setText(item1.getName() + " (" + QString::number(pixmap1.width()) + "x" + QString::number(pixmap1.height()) + ")");
    ui->rImage2->setText(item2.getName() + " (" + QString::number(pixmap2.width()) + "x" + QString::number(pixmap2.height()) + ")");
    if(pixmap1.width() >= pixmap2.width()) {
        ui->rImage1->setChecked(true);
        isFirst = true;
    } else {
        ui->rImage2->setChecked(true);
        isFirst = false;
    }

    scl = 1.0;
    scale(scl);

    connect(ui->rImage1, SIGNAL(clicked()), SLOT(checkedEvent()));
    connect(ui->rImage2, SIGNAL(clicked()), SLOT(checkedEvent()));

    connect(ui->sImage1->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(h1ChangedEvent(int)));
    connect(ui->sImage1->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(v1ChangedEvent(int)));

    connect(ui->sImage2->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(h2ChangedEvent(int)));
    connect(ui->sImage2->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(v2ChangedEvent(int)));

    connect(ui->bZoomIn, SIGNAL(clicked()), SLOT(zoomInEvent()));
    connect(ui->bZoomOut, SIGNAL(clicked()), SLOT(zoomOutEvent()));

    connect(ui->bUnite, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->bCancel, SIGNAL(clicked()), SLOT(close()));
}

UniteDialog::~UniteDialog()
{
    delete ui;
}

bool UniteDialog::getIsFirst()
{
    return isFirst;
}

void UniteDialog::h1ChangedEvent(int value)
{
    if(ui->sImage2->horizontalScrollBar()->value() != value) {
        ui->sImage2->horizontalScrollBar()->setValue(value);
    }
}

void UniteDialog::v1ChangedEvent(int value)
{
    if(ui->sImage2->verticalScrollBar()->value() != value) {
        ui->sImage2->verticalScrollBar()->setValue(value);
    }
}

void UniteDialog::h2ChangedEvent(int value)
{
    if(ui->sImage1->horizontalScrollBar()->value() != value) {
        ui->sImage1->horizontalScrollBar()->setValue(value);
    }
}

void UniteDialog::v2ChangedEvent(int value)
{
    if(ui->sImage1->verticalScrollBar()->value() != value) {
        ui->sImage1->verticalScrollBar()->setValue(value);
    }
}

void UniteDialog::checkedEvent()
{
    isFirst = (ui->rImage1->isChecked())? true: false;
}

void UniteDialog::zoomOutEvent()
{
    scl -= SCALE_STEP;
    if(scl <= MIN_SCALE) {
        scl = MIN_SCALE;
        ui->bZoomOut->setEnabled(false);
    }
    ui->bZoomIn->setEnabled(true);
    scale(scl);
}

void UniteDialog::zoomInEvent()
{
    scl += SCALE_STEP;
    if(scl >= MAX_SCALE) {
        scl = MAX_SCALE;
        ui->bZoomIn->setEnabled(false);
    }
    ui->bZoomOut->setEnabled(true);
    scale(scl);
}

void UniteDialog::scale(double scl)
{
    int w = 0;
    int h = 0;
    if(pixmap1.width() > pixmap2.width()) {
        w = pixmap1.width();
        h = pixmap1.height();
    } else {
        w = pixmap2.width();
        h = pixmap2.height();
    }
    w *= scl;
    h *= scl;
    const QPixmap& p1 = pixmap1.scaled(w, h);
    const QPixmap& p2 = pixmap2.scaled(w, h);

    ui->lImage1->setPixmap(p1);
    ui->lImage2->setPixmap(p2);
}
