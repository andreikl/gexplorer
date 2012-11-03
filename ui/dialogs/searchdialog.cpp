#include "ui/dialogs/searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget* pParent, const QString& url, const QString& path, const QString& extension): QDialog(pParent), pUi(new Ui::SearchDialog) {
    pUi->setupUi(this);

    pUi->pleUrl->setText(url);
    pUi->pleSearchPath->setText(path);
    pUi->pleExtension->setText(extension);

    connect(pUi->ppbSearch, SIGNAL(clicked()), SLOT(accept()));
    connect(pUi->ppbCancel, SIGNAL(clicked()), SLOT(close()));
}

SearchDialog::~SearchDialog() {
    delete pUi;
}

QString SearchDialog::Path() {
    return pUi->pleSearchPath->text();
}

QString SearchDialog::Extension() {
    return pUi->pleExtension->text();
}
