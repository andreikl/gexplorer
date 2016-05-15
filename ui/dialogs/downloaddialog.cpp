#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <QMessageBox>

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"
#include "data/extensiondata.h"
#include "data/gallerydata.h"

#include "ui/common/galleryitemmodel.h"

#include "ui/controls/customgallerycontrol.h"

#include "ui/dialogs/downloaddialog.h"
#include "ui_downloaddialog.h"

DownloadDialog::DownloadDialog(QWidget* parent, GalleryData& data): QDialog(parent), selectedModel(NULL), ui(new Ui::DownloadDialog)
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

    bool isSelected = false;
    if(data.getItems().count() > 0 && data.getItems().at(0)->getExtension().getType() == ExtensionData::ExtensionVideo &&
            CustomGalleryControl::getInstance() && CustomGalleryControl::getInstance()->getSelected()) {

        CustomGalleryItemData* item = CustomGalleryControl::getInstance()->getSelected();
        if(item->getItem().getExtension().getType() == ExtensionData::ExtensionVideo) {
            selectedModel = new GalleryItemModel(*item, Config::ELargeSize);
            selectedModel->loadPixmap(Config::ELargeSize);

            ui->tbCustomGallery->setIcon(QIcon(selectedModel->getPixmap()));
            ui->tbCustomGallery->setIconSize(QSize(Config::ELargeSize, Config::ELargeSize));

            isSelected = true;
        }
    }
    ui->wright->setVisible(isSelected);

}

DownloadDialog::~DownloadDialog()
{
    delete ui;

    if(selectedModel) {
        delete selectedModel;
    }
}

QString DownloadDialog::getPath()
{
    return ui->lePath->text();
}

CustomGalleryItemData* DownloadDialog::getUniteItem()
{
    if(selectedModel && ui->tbCustomGallery->isChecked()) {
        return reinterpret_cast<CustomGalleryItemData*>(selectedModel->getItem());
    }
    return NULL;
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
