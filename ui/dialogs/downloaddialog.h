#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>

class CustomGalleryItemData;
class GalleryItemModel;
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
    CustomGalleryItemData* getUniteItem();

private slots:
    void itemChangedEvent(QListWidgetItem* item);
    void itemSelectedEvent();
    void itemDelEvent();
    void itemAddEvent();

private:
    GalleryItemModel* selectedModel;
    GalleryData* data;

    Ui::DownloadDialog* ui;
};

#endif // DOWNLOADDIALOG_H
