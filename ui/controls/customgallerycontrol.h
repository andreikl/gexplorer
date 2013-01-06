#ifndef CUSTOMGALLERYCONTROL_H
#define CUSTOMGALLERYCONTROL_H

#include <QtWidgets/QWidget>

class CustomGalleryItemData;
class CustomGalleryList;
class CustomGalleryData;
class GalleryListModel;
class GalleryItemData;
class TitleControl;

QT_BEGIN_NAMESPACE
class QComboBox;
class QSplitter;
class QMenu;
QT_END_NAMESPACE

namespace Ui
{
    class CustomGalleryControl;
}

class CustomGalleryControl: public QWidget
{
    Q_OBJECT

public:
    explicit CustomGalleryControl(QWidget* parent);
    virtual ~CustomGalleryControl();

public:
    CustomGalleryData* getGallery();
    GalleryListModel* getModel();
    TitleControl* getTitle();

public:
    void selectItem(CustomGalleryItemData* value);
    void setGallery(CustomGalleryData* value);

signals:
    void onGotoItem(GalleryItemData* value);

private slots:
    void contextMenu1Event(const QPoint& pos);
    void contextMenu2Event(const QPoint& pos);

    void iconSizeChangedEvent(int value);
    void selectedEvent();
    void uniteEvent();
    void splitEvent();

    void show1Event();
    void show2Event();

    void left1Event();
    void left2Event();
    void right1Event();
    void right2Event();

    void gotoChildrenEvent(GalleryItemData* gallery);
    void gotoEvent();

    void delEvent();

    void exitCustomGallery2Event();
    void editEvent();

private:
    void create1MenuAndActions();
    void create2MenuAndActions();
    void updateButtons();

private:
    CustomGalleryData* data;

private:
    QMenu* mMenu1;
    QMenu* mMenu2;

    QComboBox* cbSize;

    QAction* aEdit;
    QAction* aUnite;
    QAction* aSplit;

    QAction* aShow1;
    QAction* aShow2;

    QAction* aLeft1;
    QAction* aLeft2;
    QAction* aRight1;
    QAction* aRight2;

    QMenu* mGotoMenu;

    QAction* aDel;

private:
    GalleryListModel* model;
    QSplitter* splitter;

    QWidget* wSplit1;
    TitleControl* title1;
    CustomGalleryList* list1;

    QWidget* wSplit2;
    TitleControl* title2;
    CustomGalleryList* list2;

private:
    Ui::CustomGalleryControl* ui;
};

#endif // CUSTOMGALLERYCONTROL_H
