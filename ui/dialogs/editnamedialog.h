#ifndef EDIT_NAME_DIALOG_H
#define EDIT_NAME_DIALOG_H

#include <QDialog>

class CustomGalleryData;
class GalleryData;
class KeyData;

namespace Ui
{
    class EditNameDialog;
}

class EditNameDialog: public QDialog {
    Q_OBJECT

public:
    EditNameDialog(QWidget* parent, CustomGalleryData& customGallery);
    EditNameDialog(QWidget* parent, GalleryData& gallery);
    EditNameDialog(QWidget* parent, KeyData& key);
    virtual ~EditNameDialog();

private:
    void init();

private slots:
    void textChangedEvent(const QString& value);

private:
    void updateButtons();

private:
    CustomGalleryData* customGallery;
    GalleryData* gallery;
    KeyData* key;

private:
    Ui::EditNameDialog* ui;
};

#endif // EDIT_NAME_DIALOG_H
