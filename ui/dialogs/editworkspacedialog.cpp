#include <QFileDialog>

#include "config.h"

#include "ui/dialogs/editworkspacedialog.h"
#include "ui_editworkspacedialog.h"

EditWorkspaceDialog::EditWorkspaceDialog(QWidget* parent): QDialog(parent), ui(new Ui::EditWorkspaceDialog)
{
    setModal(true);

    ui->setupUi(this);

    ui->pbSet->setEnabled(false);

    ui->leSource->setText(Config::getInstance()->getWorkspacePath());

    connect(ui->tbBrowse, SIGNAL(clicked()), SLOT(folderEvent()));
    connect(ui->leSource, SIGNAL(textChanged(const QString&)), SLOT(textChangedEvent(const QString&)));

    connect(ui->pbSet, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->pbCancel, SIGNAL(clicked()), SLOT(close()));
}

EditWorkspaceDialog::~EditWorkspaceDialog()
{
    delete ui;
}

QString EditWorkspaceDialog::getWorkspace()
{
    return ui->leSource->text();
}

void EditWorkspaceDialog::textChangedEvent(const QString& text)
{
    if(text.length() > 0 && text.compare(Config::getInstance()->getWorkspacePath(), Qt::CaseInsensitive)) {
        ui->pbSet->setEnabled(true);
    } else {
        ui->pbSet->setEnabled(false);
    }
}

void EditWorkspaceDialog::folderEvent()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Open Directory");
    if(!dir.isNull() && !dir.isEmpty()) {
        ui->leSource->setText(dir);
    }
}
