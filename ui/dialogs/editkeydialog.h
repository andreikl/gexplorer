#ifndef EDIT_KEYDIALOG_H
#define EDIT_KEYDIALOG_H

#include <QtGui/QDialog>

class KeyData;

namespace Ui {
    class EditKeyDialog;
}

class EditKeyDialog: public QDialog {
    Q_OBJECT

public:
    explicit EditKeyDialog(QWidget* pParent, KeyData& key);
    virtual ~EditKeyDialog();

public:
    QString Name();

private slots:
    void TextChangedEvent(const QString& value);

private:
    void UpdateButtons();

private:
    KeyData* pKey;

private:
    Ui::EditKeyDialog* pUi;
};

#endif // EDIT_KEYDIALOG_H
