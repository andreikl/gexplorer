#ifndef CUSTOMGALLERIESWINDOW_H
#define CUSTOMGALLERIESWINDOW_H

#include <QtGui/QDockWidget>

class CustomGalleriesTree;
class CustomGalleryData;
class TitleControl;

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

namespace Ui {
    class CustomGalleriesWindow;
}

class CustomGalleriesWindow: public QDockWidget {
    Q_OBJECT

public:
    explicit CustomGalleriesWindow(QWidget* pParent);
    virtual ~CustomGalleriesWindow();

public:
    void selectGallery(CustomGalleryData* value);

signals:
    void OnGallery(CustomGalleryData* value);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void AddEvent();
    void DelEvent();
    void EditEvent();
    void ExitEvent();

private slots:
    void ActivatedEvent();
    void SelectedEvent();

private:
    void CreateMenuAndActions();
    void UpdateButtons();

private:
    QMenu* pmMenu;
    QAction* paAdd;
    QAction* paShow;
    QAction* paEdit;
    QAction* paDel;

private:
    TitleControl* pTitle;
    CustomGalleriesTree* ptwGalleries;

private:
    Ui::CustomGalleriesWindow* pUi;
};

#endif // CUSTOMGALLERIESWINDOW_H
