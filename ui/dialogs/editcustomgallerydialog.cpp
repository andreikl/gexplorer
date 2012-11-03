#include "data/customgallerydata.h"

#include "ui/dialogs/editcustomgallerydialog.h"

#include "ui_editcustomgallerydialog.h"

EditCustomGalleryDialog::EditCustomGalleryDialog(QWidget* pParent, CustomGalleryData& gallery): QDialog(pParent), pUi(new Ui::EditCustomGalleryDialog) {
    pGallery = &gallery;

    setModal(true);
    pUi->setupUi(this);

    pUi->pleName->setText(pGallery->getName());
    connect(pUi->pleName, SIGNAL(textChanged(const QString&)), SLOT(TextChangedEvent(const QString&)));

    connect(pUi->ppbSave, SIGNAL(clicked()), SLOT(accept()));
    connect(pUi->ppbCancel, SIGNAL(clicked()), SLOT(close()));

    UpdateButtons();
}

EditCustomGalleryDialog::~EditCustomGalleryDialog() {
    delete pUi;
}

void EditCustomGalleryDialog::TextChangedEvent(const QString& value) {
    pGallery->setName(value);
    UpdateButtons();
}

void EditCustomGalleryDialog::UpdateButtons() {
    if(pGallery->getName().isEmpty()) {
        pUi->ppbSave->setEnabled(false);
    } else {
        pUi->ppbSave->setEnabled(true);
    }
}
