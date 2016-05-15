#ifndef VIEWERDIALOG_H
#define VIEWERDIALOG_H

#include <QDialog>

class GalleryListModel;
class ViewerControl;
class TitleControl;

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

namespace Ui
{
    class ViewerDialog;
}

class ViewerDialog: public QDialog
{
    Q_OBJECT
    
public:
    explicit ViewerDialog(QWidget* parent, GalleryListModel& model, int row);
    virtual ~ViewerDialog();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void currentEvent();
    void slideEvent();
    void closeEvent();
    void backEvent();
    void nextEvent();
    void tickEvent();

private:
    void createMenuAndActions();
    void updateButtons();

private:
    bool isSlide;

private:
    QAction* aSlide;
    QAction* aBack;
    QAction* aNext;
    QMenu* mMenu;

private:
    ViewerControl* viewer;
    TitleControl* title;

private:
    Ui::ViewerDialog* ui;
};

#endif // VIEWERDIALOG_H
