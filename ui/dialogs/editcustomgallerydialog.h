#ifndef EDIT_CUSTOM_GALLERY_DIALOG_H
#define EDIT_CUSTOM_GALLERY_DIALOG_H

#include <QtGui/QDialog>

class CustomGalleryData;

namespace Ui {
    class EditCustomGalleryDialog;
}

class EditCustomGalleryDialog: public QDialog {
    Q_OBJECT

public:
    explicit EditCustomGalleryDialog(QWidget* pParent, CustomGalleryData& gallery);
    virtual ~EditCustomGalleryDialog();

private slots:
    void TextChangedEvent(const QString& value);

private:
    void UpdateButtons();

private:
    CustomGalleryData* pGallery;

private:
    Ui::EditCustomGalleryDialog* pUi;
};

#endif // EDIT_CUSTOM_GALLERY_DIALOG_H
