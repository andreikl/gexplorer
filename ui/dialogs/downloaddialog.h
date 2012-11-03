#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QtGui/QDialog>

class GalleryData;

namespace Ui {
    class DownloadDialog;
}

class DownloadDialog : public QDialog {
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget* pParent, GalleryData& data);
    virtual ~DownloadDialog();

public:
    QString Url();
    QString Path();

private:
    Ui::DownloadDialog* pUi;
};

#endif // DOWNLOADDIALOG_H
