#ifndef ADDGALLERYDIALOG_H
#define ADDGALLERYDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class AddGalleryDialog;
}

class AddGalleryDialog: public QDialog {
    Q_OBJECT

public:
    explicit AddGalleryDialog(QWidget* pParent = NULL);
    virtual ~AddGalleryDialog();

public:
    QString Source();

private slots:
    void TextChangedEvent(const QString& text);
    void FolderEvent();

private:
    Ui::AddGalleryDialog* pUi;
};

#endif // ADDGALLERYDIALOG_H
