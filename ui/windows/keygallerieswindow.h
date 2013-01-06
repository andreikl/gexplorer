#ifndef KEYGALLERIESWINDOW_H
#define KEYGALLERIESWINDOW_H

#include <QtWidgets/QDockWidget>

class CustomGalleryData;
class TitleControl;
class KeysTree;
class KeyData;

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

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

public:
    void selectKey(KeyData* key, CustomGalleryData* gallery);

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
    void createMenuAndActions(TitleControl* title);
    void updateButtons();

private:
    QAction* aShowGallery;
    QAction* aGotoGallery;
    QAction* aDelGallery;
    QAction* aEditKey;
    QAction* aAddKey;
    QAction* aDelKey;
    QMenu* mMenu;

private:
    KeysTree* twKeys;

private:
    Ui::KeyGalleriesWindow* ui;

};

#endif // KEYGALLERIESWINDOW_H
