#include "ui/dialogs/searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QWidget* parent, const QString& url, const QString& path, const QString& extension): QDialog(parent), ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    ui->leUrl->setText(url);
    ui->leSearchPath->setText(path);
    ui->leExtension->setText(extension);

    connect(ui->pbSearch, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked()), SLOT(close()));
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

QString SearchDialog::getExtension()
{
    return ui->leExtension->text();
}

QString SearchDialog::getPath()
{
    return ui->leSearchPath->text();
}
