#ifndef CUSTOMGALLERIESWINDOW_H
#define CUSTOMGALLERIESWINDOW_H

#include <QDockWidget>

class CustomGalleriesTree;
class CustomGalleryData;
class TitleControl;
class KeyData;

QT_BEGIN_NAMESPACE
class QComboBox;
class QMenu;
QT_END_NAMESPACE

namespace Ui
{
    class CustomGalleriesWindow;
}

class CustomGalleriesWindow: public QDockWidget
{
    Q_OBJECT

public:
    explicit CustomGalleriesWindow(QWidget* parent);
    virtual ~CustomGalleriesWindow();

public:
    void selectGallery(CustomGalleryData* value);

signals:
    void onGotoKey(KeyData*, CustomGalleryData*);
    void onGallery(CustomGalleryData* value);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void filterChangedEvent(int value);
    void activatedEvent();
    void selectedEvent();

    void gotoKeyEvent(KeyData* key, CustomGalleryData* gallery);
    void gotoEvent();

    void editEvent();
    void exitEvent();
    void addEvent();
    void delEvent();

private:
    void createMenuAndActions(TitleControl* title);
    void updateButtons();

private:
    QMenu* mMenu;

    QComboBox* cbFilter;
    QMenu* mGotoMenu;

    QAction* aShow;
    QAction* aEdit;
    QAction* aAdd;
    QAction* aDel;

private:
    CustomGalleriesTree* twGalleries;

private:
    Ui::CustomGalleriesWindow* ui;
};

#endif // CUSTOMGALLERIESWINDOW_H
