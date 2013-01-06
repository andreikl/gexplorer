#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QAction>

#include "helpers/gstreamerhelper.h"

#include "ui/controls/videocontrol.h"

VideoControl::VideoControl(QWidget* parent): QWidget(parent)
{
    callTimer = new QTimer(this);
    callTimer->setInterval(200);
    connect(callTimer, SIGNAL(timeout()), SLOT(callEvent()));

    wArea = new QWidget(this);

    wVideoArea = new QWidget(wArea);
    //wVideoArea->setAttribute(Qt::WA_NativeWindow, true);

    tbPlayer = new QToolBar(this);

    aPlay = new QAction("&Show", this);
    aPlay->setStatusTip("Show active item");
    aPlay->setIcon(QIcon(":/res/resources/pause.png"));
    aPlay->setCheckable(true);
    connect(aPlay, SIGNAL(triggered()), this, SLOT(playEvent()));

    sSlider = new QSlider(this);
    sSlider->setOrientation(Qt::Horizontal);
    sSlider->setRange(0, 1000);
    connect(sSlider, SIGNAL(sliderMoved(int)), SLOT(movedEvent(int)));

    tbPlayer->addAction(aPlay);
    tbPlayer->addWidget(sSlider);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(wArea);
    layout->addWidget(tbPlayer);
}

VideoControl::~VideoControl()
{
    if(callTimer->isActive()) {
        callTimer->stop();
    }
    delete callTimer;

    GStreamerHelper::getInstance()->stopPlay();
}

void VideoControl::startPlay(const QString& path, int w, int h)
{
    GStreamerHelper::getInstance()->startPlay(wVideoArea, wArea->width(), wArea->height(), path, w, h);
    if(!callTimer->isActive()) {
        callTimer->start();
    }
}

void VideoControl::movedEvent(int pos)
{
    long int p = 0;
    long int l = 0;
    GStreamerHelper::getInstance()->getPosAndLen(&p, &l);
    if(p != 0 && l != 0) {
        GStreamerHelper::getInstance()->setPos(pos * l / (double)sSlider->maximum());
    }
}

void VideoControl::playEvent()
{
    if(aPlay->isChecked()) {
        GStreamerHelper::getInstance()->pause();
    } else {
        GStreamerHelper::getInstance()->play();
    }

}

void VideoControl::callEvent()
{
    long int p = 0;
    long int l = 0;
    GStreamerHelper::getInstance()->getPosAndLen(&p, &l);
    if(p != 0 && l != 0) {
        sSlider->setSliderPosition(p * (double)sSlider->maximum() / (double)l);
    }
}
