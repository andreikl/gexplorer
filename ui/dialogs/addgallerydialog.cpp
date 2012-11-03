#include <QtGui/QFileDialog>

#include "ui/dialogs/addgallerydialog.h"
#include "ui_addgallerydialog.h"

AddGalleryDialog::AddGalleryDialog(QWidget* pParent): QDialog(pParent), pUi(new Ui::AddGalleryDialog) {
    setModal(true);

    pUi->setupUi(this);

    pUi->ppbAdd->setEnabled(false);

    connect(pUi->ptbBrowse, SIGNAL(clicked()), SLOT(FolderEvent()));
    connect(pUi->pleSource, SIGNAL(textChanged(const QString&)), SLOT(TextChangedEvent(const QString&)));

    connect(pUi->ppbAdd, SIGNAL(clicked()), SLOT(accept()));
    connect(pUi->ppbCancel, SIGNAL(clicked()), SLOT(close()));
}

AddGalleryDialog::~AddGalleryDialog() {
    delete pUi;
}

QString AddGalleryDialog::Source() {
    return pUi->pleSource->text();
}

void AddGalleryDialog::TextChangedEvent(const QString& text) {
    if(text.length() > 0) {
        pUi->ppbAdd->setEnabled(true);
    } else {
        pUi->ppbAdd->setEnabled(false);
    }
}

void AddGalleryDialog::FolderEvent() {
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory");
    if(!dir.isNull() && !dir.isEmpty()) {
        pUi->pleSource->setText(dir);
    }
}
