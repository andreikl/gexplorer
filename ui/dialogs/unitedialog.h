#ifndef UNITEDIALOG_H
#define UNITEDIALOG_H

#include <QtGui/QDialog>

class CustomGalleryItemData;

namespace Ui {
    class UniteDialog;
}

class UniteDialog: public QDialog {
    Q_OBJECT

public:
    explicit UniteDialog(QWidget* pParent, CustomGalleryItemData& item1, CustomGalleryItemData& item2);
    virtual ~UniteDialog();

public:
    bool IsFirst();

private slots:
    void OnH1Changed(int value);
    void OnV1Changed(int value);
    void OnH2Changed(int value);
    void OnV2Changed(int value);
    void OnChecked();
    void OnZoomOut();
    void OnZoomIn();

private:
    void Scale(double scale);

private:
    QPixmap pixmap1;
    QPixmap pixmap2;
    bool isFirst;
    double scale;

private:
    Ui::UniteDialog* pUi;
};

#endif // UNITEDIALOG_H
