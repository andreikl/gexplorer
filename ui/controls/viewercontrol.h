#ifndef VIEWERCONTROL_H
#define VIEWERCONTROL_H

#include <QWidget>

class GalleryListModel;
class ViewerDialog;
class VideoControl;

class ViewerControl: public QWidget
{
    Q_OBJECT

public:
    ViewerControl(ViewerDialog* parent, const GalleryListModel& m, int row);
    ~ViewerControl();

public:
    const GalleryListModel& getModel();
    int getCurrentRow();

public:
    void currentEvent();
    void backEvent();
    void nextEvent();

protected:
    void paintEvent(QPaintEvent* event);

private:
    const GalleryListModel& model;
    int currentRow;

    VideoControl* video;
    QPixmap pixmap;
};

#endif // VIEWERCONTROL_H
