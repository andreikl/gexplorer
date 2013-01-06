#include <QtWidgets/QFileDialog>

#include "ui/dialogs/addgallerydialog.h"
#include "ui_addgallerydialog.h"

AddGalleryDialog::AddGalleryDialog(QWidget* parent): QDialog(parent), ui(new Ui::AddGalleryDialog)
{
    setModal(true);

    ui->setupUi(this);

    ui->pbAdd->setEnabled(false);

    connect(ui->tbBrowse, SIGNAL(clicked()), SLOT(folderEvent()));
    connect(ui->leSource, SIGNAL(textChanged(const QString&)), SLOT(textChangedEvent(const QString&)));

    connect(ui->pbAdd, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked()), SLOT(close()));
}

AddGalleryDialog::~AddGalleryDialog()
{
    delete ui;
}

QString AddGalleryDialog::getSource()
{
    return ui->leSource->text();
}

void AddGalleryDialog::textChangedEvent(const QString& text)
{
    if(text.length() > 0) {
        ui->pbAdd->setEnabled(true);
    } else {
        ui->pbAdd->setEnabled(false);
    }
}

void AddGalleryDialog::folderEvent()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory");
    if(!dir.isNull() && !dir.isEmpty()) {
        ui->leSource->setText(dir);
    }
}
