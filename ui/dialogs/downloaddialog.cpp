#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "ui/dialogs/downloaddialog.h"

#include "ui_downloaddialog.h"

DownloadDialog::DownloadDialog(QWidget* pParent, GalleryData& data): QDialog(pParent), pUi(new Ui::DownloadDialog) {
    pUi->setupUi(this);

    pUi->plePath->setText(data.getPath());

    foreach(GalleryItemData* pItem, data.getItems()) {
        pUi->plwUrls->addItem(pItem->getFileName());
    }

    connect(pUi->ppbDownload, SIGNAL(clicked()), SLOT(accept()));
    connect(pUi->ppbCancel, SIGNAL(clicked()), SLOT(close()));
}

DownloadDialog::~DownloadDialog() {
    delete pUi;
}

QString DownloadDialog::Path() {
    return pUi->plePath->text();
}
