#ifndef GALLERYCONTROL_H
#define GALLERYCONTROL_H

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QMenu;
QT_END_NAMESPACE

class GalleryListModel;
class TitleControl;
class GalleryData;
class GalleryList;

namespace Ui {
    class GalleryControl;
}

class GalleryControl: public QWidget {
    Q_OBJECT

public:
    explicit GalleryControl(QWidget* pParent = NULL);
    virtual ~GalleryControl();

public:
    GalleryListModel* Model();
    TitleControl* Title();

public:
    void Gallery(GalleryData* pValue);
    const GalleryData* Gallery() const;

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private slots:
    void IconSizeChangedEvent(int value);
    void ShowEvent();

private slots:
    void SelectedEvent();

private:
    void CreateMenuAndActions();
    void UpdateButtons();

private:
    GalleryData* pData;

private:
    QMenu* pmMenu;
    QComboBox* pcbSize;
    QAction* paShow;

private:
    GalleryListModel* pModel;
    TitleControl* pTitle;
    GalleryList* pList;

private:
    Ui::GalleryControl* pUi;
};

#endif // GALLERYCONTROL_H
