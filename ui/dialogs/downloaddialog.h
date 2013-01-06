#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QtWidgets/QDialog>

class GalleryData;

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

namespace Ui
{
    class DownloadDialog;
}

class DownloadDialog: public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget* parent, GalleryData& data);
    virtual ~DownloadDialog();

public:
    QString getPath();

private slots:
    void itemChangedEvent(QListWidgetItem* item);
    void itemSelectedEvent();
    void itemDelEvent();
    void itemAddEvent();

private:
    Ui::DownloadDialog* ui;
    GalleryData* data;
};

#endif // DOWNLOADDIALOG_H
