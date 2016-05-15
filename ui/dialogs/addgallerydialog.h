#ifndef ADDGALLERYDIALOG_H
#define ADDGALLERYDIALOG_H

#include <QDialog>

namespace Ui
{
    class AddGalleryDialog;
}

class AddGalleryDialog: public QDialog
{
    Q_OBJECT

public:
    explicit AddGalleryDialog(QWidget* parent);
    virtual ~AddGalleryDialog();

public:
    QString getSource();

private slots:
    void textChangedEvent(const QString& text);
    void folderEvent();

private:
    Ui::AddGalleryDialog* ui;
};

#endif // ADDGALLERYDIALOG_H
