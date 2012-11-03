#ifndef CUSTOMGALLERYCONTROL_H
#define CUSTOMGALLERYCONTROL_H

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QMenu;
QT_END_NAMESPACE

class CustomGalleryList;
class CustomGalleryData;
class GalleryListModel;
class TitleControl;

namespace Ui {
    class CustomGalleryControl;
}

class CustomGalleryControl: public QWidget {
    Q_OBJECT

public:
    explicit CustomGalleryControl(QWidget* pParent = NULL);
    virtual ~CustomGalleryControl();

public:
    GalleryListModel* Model();
    TitleControl* Title();

public:
    void Gallery(CustomGalleryData* pValue);
    CustomGalleryData* Gallery();

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private slots:
    void IconSizeChangedEvent(int value);
    void ShowEvent();
    void LeftEvent();
    void RightEvent();
    void UniteEvent();
    void DelEvent();

private slots:
    void SelectedEvent();

private:
    void CreateMenuAndActions();
    void UpdateButtons();

private:
    CustomGalleryData* pData;

private:
    QMenu* pmMenu;
    QComboBox* pcbSize;
    QAction* paShow;
    QAction* paLeft;
    QAction* paRight;
    QAction* paUnite;
    QAction* paDel;

private:
    GalleryListModel* pModel;
    CustomGalleryList* pList;
    TitleControl* pTitle;

private:
    Ui::CustomGalleryControl* pUi;
};

#endif // CUSTOMGALLERYCONTROL_H
