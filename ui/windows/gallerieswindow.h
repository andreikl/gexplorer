#ifndef GALLERIESWINDOW_H
#define GALLERIESWINDOW_H

#include <QtGui/QDockWidget>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class GalleryTreeModel;
class GalleriesTree;
class TitleControl;
class GalleryData;

namespace Ui {
    class GalleriesWindow;
}

class GalleriesWindow: public QDockWidget {
    Q_OBJECT

public:
    explicit GalleriesWindow(QWidget* pParent = NULL);
    virtual ~GalleriesWindow();

signals:
    void OnGallery(GalleryData* pData);
    void OnBrowse(GalleryData* pData);
    void OnAdd(const QString& source);

protected:
    void contextMenuEvent(QContextMenuEvent*);

private slots:
    void AddEvent();
    void BrowseEvent();
    void DelEvent();
    void ExitEvent();

private slots:
    void AddGalleryEvent(GalleryTreeModel*, GalleryData* pGallery);
    void ActivatedEvent();
    void SelectedEvent();

private:
    void CreateMenuAndActions();
    void UpdateButtons();

private:
    QMenu* pmMenu;
    QAction* paAdd;
    QAction* paShow;
    QAction* paBrowse;
    QAction* paDel;

private:
    TitleControl* pTitle;
    GalleriesTree* ptwGalleries;

private:
    Ui::GalleriesWindow *pUi;

};

#endif // GALLERIESWINDOW_H
