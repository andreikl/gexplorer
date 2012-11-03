#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "application.h"

#include "data/gallerydata.h"

#include "ui/controls/cleancontrol.h"
#include "ui/controls/browsercontrol.h"
#include "ui/controls/gallerycontrol.h"
#include "ui/controls/customgallerycontrol.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* pParent = NULL);
    virtual ~MainWindow();

private slots:
    void customGalleriesWindowEvent(bool value);
    void keyGalleriesWindowEvent(bool value);
    void galleriesWindowEvent(bool value);
    void downloadsWindowEvent(bool value);

private slots:
    void gotoCustomGalleryEvent(CustomGalleryData* value);
    void customGalleryEvent(CustomGalleryData* value);

private slots:
    void delCustomGalleryEvent(CustomGalleryData* gallery);
    void delGalleryEvent(GalleryData* gallery);

private slots:
    void AddGalleryEvent(const QString& source);
    void BrowseGalleryEvent(GalleryData* pValue);
    void GalleryEvent(GalleryData* pValue);

private slots:
    void ExitBrowserEvent();
    void ExitGalleryEvent();
    void ExitCustomGalleryEvent();

private:
    BrowserControl* pBrowser;
    bool isBrowser;

private:
    GalleryControl* pGallery;
    bool isGallery;

private:
    CustomGalleryControl* pCustomGallery;
    bool isCustomGallery;

private:
    CleanControl* pClean;
    Ui::MainWindow* pUi;
};

#endif // MAINWINDOW_H
