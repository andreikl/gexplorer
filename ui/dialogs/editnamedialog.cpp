#include "data/customgallerydata.h"
#include "data/gallerydata.h"
#include "data/keydata.h"

#include "ui/dialogs/editnamedialog.h"
#include "ui_editnamedialog.h"

EditNameDialog::EditNameDialog(QWidget* parent, CustomGalleryData& customGallery): QDialog(parent), ui(new Ui::EditNameDialog)
{
    init();

    this->customGallery = &customGallery;

    ui->leName->setText(customGallery.getName());

    updateButtons();
}

EditNameDialog::EditNameDialog(QWidget* parent, GalleryData& gallery): QDialog(parent), ui(new Ui::EditNameDialog)
{
    init();

    this->gallery = &gallery;

    ui->leName->setText(gallery.getSource());
    //ui->leName->setReadOnly(true);

    updateButtons();
}

EditNameDialog::EditNameDialog(QWidget* parent, KeyData& key): QDialog(parent), ui(new Ui::EditNameDialog)
{
    init();

    this->key = &key;

    ui->leName->setText(key.getName());

    updateButtons();
}

void EditNameDialog::init() {
    customGallery = NULL;
    gallery = NULL;
    key = NULL;

    setModal(true);
    ui->setupUi(this);

    connect(ui->leName, SIGNAL(textChanged(const QString&)), SLOT(textChangedEvent(const QString&)));
    connect(ui->pbSave, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked()), SLOT(close()));
}

EditNameDialog::~EditNameDialog()
{
    delete ui;
}

void EditNameDialog::textChangedEvent(const QString& value)
{
    if(customGallery) {
        customGallery->setName(value);
    } else if(gallery) {
        gallery->setSource(value);
    } else if(key) {
        key->setName(value);
    }
    updateButtons();
}

void EditNameDialog::updateButtons()
{
    if(customGallery && !customGallery->getName().isEmpty()) {
        ui->pbSave->setEnabled(true);
    } else if(gallery && !gallery->getSource().isEmpty()) {
        ui->pbSave->setEnabled(true);
    } else if(key && !key->getName().isEmpty()) {
        ui->pbSave->setEnabled(true);
    } else {
        ui->pbSave->setEnabled(false);
    }
}
