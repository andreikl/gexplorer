#ifndef UNITEDIALOG_H
#define UNITEDIALOG_H

#include <QtWidgets/QDialog>

class CustomGalleryItemData;

namespace Ui
{
    class UniteDialog;
}

class UniteDialog: public QDialog
{
    Q_OBJECT

public:
    explicit UniteDialog(QWidget* parent, CustomGalleryItemData& item1, CustomGalleryItemData& item2);
    virtual ~UniteDialog();

public:
    bool getIsFirst();

private slots:
    void h1ChangedEvent(int value);
    void v1ChangedEvent(int value);
    void h2ChangedEvent(int value);
    void v2ChangedEvent(int value);
    void checkedEvent();
    void zoomOutEvent();
    void zoomInEvent();

private:
    void scale(double scl);

private:
    QPixmap pixmap1;
    QPixmap pixmap2;
    bool isFirst;
    double scl;

private:
    Ui::UniteDialog* ui;
};

#endif // UNITEDIALOG_H
