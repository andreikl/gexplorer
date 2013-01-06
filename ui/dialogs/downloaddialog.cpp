#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "ui/dialogs/downloaddialog.h"
#include "ui_downloaddialog.h"

DownloadDialog::DownloadDialog(QWidget* parent, GalleryData& data): QDialog(parent), ui(new Ui::DownloadDialog)
{
    this->data = &data;

    ui->setupUi(this);

    ui->lePath->setText(data.getPath());

    foreach(GalleryItemData* item, data.getItems()) {
        QListWidgetItem* i = new QListWidgetItem(item->getUrl());
        i->setFlags(i->flags() | Qt::ItemIsEditable);
        i->setData(Qt::UserRole, qVariantFromValue(item));
        ui->lwUrls->addItem(i);
    }
    connect(ui->lwUrls, SIGNAL(itemChanged(QListWidgetItem*)), SLOT(itemChangedEvent(QListWidgetItem*)));
    connect(ui->lwUrls, SIGNAL(itemSelectionChanged()), SLOT(itemSelectedEvent()));

    connect(ui->tbDelete, SIGNAL(clicked()), SLOT(itemDelEvent()));
    connect(ui->tbAdd, SIGNAL(clicked()), SLOT(itemAddEvent()));

    connect(ui->pbDownload, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked()), SLOT(close()));

    itemSelectedEvent();
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

QString DownloadDialog::getPath()
{
    return ui->lePath->text();
}

void DownloadDialog::itemChangedEvent(QListWidgetItem* item)
{
    GalleryItemData* i = qvariant_cast<GalleryItemData*>(item->data(Qt::UserRole));
    if(item->text() != i->getUrl()) {
        i->setUrl(item->text());
        QFileInfo info(item->text());
        i->setFileName(info.fileName());
    }
}

void DownloadDialog::itemSelectedEvent()
{
    if(ui->lwUrls->selectedItems().count() == 1) {
        ui->tbDelete->setEnabled(true);
        ui->tbAdd->setEnabled(true);
    } else {
        ui->tbDelete->setEnabled(false);
        ui->tbAdd->setEnabled(false);
    }
}

void DownloadDialog::itemDelEvent()
{
    if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
        QListWidgetItem* item = ui->lwUrls->selectedItems()[0];
        GalleryItemData* i = qvariant_cast<GalleryItemData*>(item->data(Qt::UserRole));
        delete item;
        delete i;
    }
}

void DownloadDialog::itemAddEvent()
{
    QListWidgetItem* item = ui->lwUrls->selectedItems()[0];
    GalleryItemData* i = qvariant_cast<GalleryItemData*>(item->data(Qt::UserRole));

    GalleryItemData* itemClone = i->clone(this->data);

    QListWidgetItem* itemList = new QListWidgetItem(itemClone->getUrl());
    itemList->setFlags(itemList->flags() | Qt::ItemIsEditable);
    itemList->setData(Qt::UserRole, qVariantFromValue(itemClone));
    ui->lwUrls->addItem(itemList);

}
