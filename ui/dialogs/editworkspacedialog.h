#ifndef EDITWORKSPACEDIALOG_H
#define EDITWORKSPACEDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui
{
    class EditWorkspaceDialog;
}

class EditWorkspaceDialog: public QDialog
{
    Q_OBJECT

public:
    explicit EditWorkspaceDialog(QWidget* parent);
    virtual ~EditWorkspaceDialog();

public:
    QString getWorkspace();

private slots:
    void textChangedEvent(const QString& text);
    void folderEvent();

private:
    Ui::EditWorkspaceDialog* ui;
};

#endif // EDITWORKSPACEDIALOG_H
