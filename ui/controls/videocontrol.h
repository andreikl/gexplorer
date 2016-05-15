#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolBar;
class QSlider;
QT_END_NAMESPACE

class VideoControl: public QWidget
{
    Q_OBJECT

public:
    VideoControl(QWidget* parent);
    ~VideoControl();

public:
    void startPlay(const QString& path, int w, int h);

private slots:
    void movedEvent(int pos);
    void callEvent();
    void playEvent();

private:
    QTimer* callTimer;

    QWidget* wVideoArea;

    QToolBar* tbPlayer;
    QSlider* sSlider;
    QAction* aPlay;
    QWidget* wArea;

};

#endif // VIDEOCONTROL_H
