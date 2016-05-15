#ifndef GALLERYCONTROL_H
#define GALLERYCONTROL_H

#include <QWidget>

class CustomGalleryItemData;
class GalleryListModel;
class GalleryItemData;
class TitleControl;
class GalleryData;
class GalleryList;

QT_BEGIN_NAMESPACE
class QComboBox;
class QMenu;
QT_END_NAMESPACE

namespace Ui
{
    class GalleryControl;
}

class GalleryControl: public QWidget
{
    Q_OBJECT

public:
    explicit GalleryControl(QWidget* parent);
    virtual ~GalleryControl();

public:
    const GalleryData* getGallery() const;
    GalleryListModel* getModel();
    TitleControl* getTitle();

public:
    void selectItem(GalleryItemData* value);
    void setGallery(GalleryData* value);

signals:
    void onGotoItem(CustomGalleryItemData* value);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void iconSizeChangedEvent(int value);
    void selectedEvent();
    void showEvent();

    void gotoEvent(CustomGalleryItemData* gallery);
    void gotoEvent();

    void delEvent();

private:
    void createMenuAndActions();
    void updateButtons();

private:
    GalleryData* data;

private:
    QMenu* mMenu;

    QComboBox* cbSize;
    QAction* aShow;
    QMenu* mGotoMenu;
    QAction* aDel;

private:
    GalleryListModel* model;
    TitleControl* title;
    GalleryList* list;

private:
    Ui::GalleryControl* ui;
};

#endif // GALLERYCONTROL_H
