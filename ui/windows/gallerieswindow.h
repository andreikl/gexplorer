#ifndef GALLERIESWINDOW_H
#define GALLERIESWINDOW_H

#include <QtWidgets/QDockWidget>

class GalleryTreeModel;
class GalleriesTree;
class TitleControl;
class GalleryData;

QT_BEGIN_NAMESPACE
class QComboBox;
class QMenu;
QT_END_NAMESPACE

namespace Ui
{
    class GalleriesWindow;
}

class GalleriesWindow: public QDockWidget
{
    Q_OBJECT

public:
    explicit GalleriesWindow(QWidget* parent);
    virtual ~GalleriesWindow();

public:
    void selectGallery(GalleryData* value);

signals:
    void onAdd(const QString& source);
    void onGallery(GalleryData* data);
    void onBrowse(GalleryData* data);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void addGalleryEvent(GalleryData* gallery);
    void filterChangedEvent(int value);
    void activatedEvent();
    void selectedEvent();
    void browseEvent();
    void editEvent();
    void exitEvent();
    void addEvent();
    void delEvent();

private:
    void createMenuAndActions(TitleControl* title);
    void updateButtons();

private:
    QComboBox* cbFilter;

    QAction* aGotoGallery;
    QAction* aBrowse;
    QAction* aEdit;
    QAction* aShow;
    QAction* aAdd;
    QAction* aDel;
    QMenu* mMenu;

private:
    GalleriesTree* twGalleries;

private:
    Ui::GalleriesWindow* ui;

};

#endif // GALLERIESWINDOW_H
