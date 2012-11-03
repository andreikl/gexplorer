#ifndef VIEWERDIALOG_H
#define VIEWERDIALOG_H

#include <QtGui/QDialog>

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class GalleryListModel;
class ViewerControl;
class TitleControl;

namespace Ui {
    class ViewerDialog;
}

class ViewerDialog: public QDialog {
    Q_OBJECT
    
public:
    explicit ViewerDialog(QWidget* pParent, GalleryListModel& model, int row);
    virtual ~ViewerDialog();

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private slots:
    void BackEvent();
    void NextEvent();
    void CurrentEvent();

private slots:
    void TickEvent();
    void SlideEvent();
    void CloseEvent();

private:
    void CreateMenuAndActions();
    void UpdateButtons();

private:
    bool isSlide;

private:
    QMenu* pmMenu;
    QAction* paBack;
    QAction* paNext;
    QAction* paSlide;

private:
    ViewerControl* pViewer;
    TitleControl* pTitle;

private:
    Ui::ViewerDialog* pUi;
};

#endif // VIEWERDIALOG_H
