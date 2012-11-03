#include "data/keydata.h"

#include "ui/dialogs/editkeydialog.h"

#include "ui_editkeydialog.h"

EditKeyDialog::EditKeyDialog(QWidget* pParent, KeyData& key): QDialog(pParent), pUi(new Ui::EditKeyDialog) {
    pKey = &key;

    setModal(true);
    pUi->setupUi(this);

    pUi->pleName->setText(pKey->getName());
    connect(pUi->pleName, SIGNAL(textChanged(const QString&)), SLOT(TextChangedEvent(const QString&)));

    connect(pUi->ppbSave, SIGNAL(clicked()), SLOT(accept()));
    connect(pUi->ppbCancel, SIGNAL(clicked()), SLOT(close()));

    UpdateButtons();
}

EditKeyDialog::~EditKeyDialog() {
    delete pUi;
}

void EditKeyDialog::TextChangedEvent(const QString& value) {
    pKey->setName(value);
    UpdateButtons();
}

void EditKeyDialog::UpdateButtons() {
    if(pKey->getName().isEmpty()) {
        pUi->ppbSave->setEnabled(false);
    } else {
        pUi->ppbSave->setEnabled(true);
    }
}
