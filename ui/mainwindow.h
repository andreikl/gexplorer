#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CustomGalleriesWindow;
class KeyGalleriesWindow;
class GalleriesWindow;
class DownloadsWindow;

class BrowserControl;
class GalleryControl;

class CustomGalleryItemData;
class CustomGalleryData;
class GalleryItemData;
class GalleryData;
class KeyData;

QT_BEGIN_NAMESPACE
class QSplitter;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent);
    virtual ~MainWindow();

private slots:
    void editWorkspaceEvent();
    void loadModelEvent();

    void gotoCustomGalleryItemEvent(CustomGalleryItemData* item);
    void gotoCustomGalleryEvent(CustomGalleryData* value);
    void gotoGalleryItemEvent(GalleryItemData* item);
    void gotoKeyEvent(KeyData* key, CustomGalleryData* gallery);

    void customGalleryEvent(CustomGalleryData* value);
    void browseGalleryEvent(GalleryData* value);
    void galleryEvent(GalleryData* value);

    void addGalleryEvent(const QString& source);

    void delCustomGalleryEvent(CustomGalleryData* gallery);
    void delGalleryEvent(GalleryData* gallery);

    void customGalleriesWindowEvent(bool value);
    void keyGalleriesWindowEvent(bool value);
    void galleriesWindowEvent(bool value);
    void downloadsWindowEvent(bool value);

    void exitCustomGalleryEvent();
    void exitGalleryEvent();
    void exitBrowserEvent();

private:
    void showBrowser();

    void unloadWindows();
    void loadWindows();

private:
    GalleriesWindow* dwGalleries;
    CustomGalleriesWindow* dwCustomGalleries;
    KeyGalleriesWindow* dwKeys;
    DownloadsWindow* dwDownloads;

    GalleryControl* gallery;
    BrowserControl* browser;

private:
    bool isCustomGallery;
    bool isGallery;
    bool isBrowser;

private:
    QSplitter* splitter;
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
