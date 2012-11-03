#ifndef KEYGALLERIESWINDOW_H
#define KEYGALLERIESWINDOW_H

#include <QtGui/QDockWidget>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class CustomGalleryData;
class TitleControl;
class KeysTree;

namespace Ui
{
    class KeyGalleriesWindow;
}

class KeyGalleriesWindow: public QDockWidget
{
    Q_OBJECT

public:
    explicit KeyGalleriesWindow(QWidget* parent);
    virtual ~KeyGalleriesWindow();

signals:
    void onGotoGallery(CustomGalleryData* value);
    void onGallery(CustomGalleryData* value);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void activatedEvent();
    void selectedEvent();
    void gotoEvent();
    void editEvent();
    void exitEvent();
    void addEvent();
    void delEvent();

private:
    void createMenuAndActions();
    void updateButtons();

private:
    QMenu* mMenu;
    QAction* aAddKey;
    QAction* aEditKey;
    QAction* aDelKey;
    QAction* aShowGallery;
    QAction* aDelGallery;
    QAction* aGotoGallery;

private:
    TitleControl* title;
    KeysTree* twKeys;

private:
    Ui::KeyGalleriesWindow* ui;

};

#endif // KEYGALLERIESWINDOW_H
