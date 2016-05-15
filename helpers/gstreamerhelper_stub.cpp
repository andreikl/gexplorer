#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "helpers/gstreamerhelper.h"

GStreamerHelper* GStreamerHelper::instance = NULL;

GStreamerHelper* GStreamerHelper::getInstance()
{
    return instance;
}

GStreamerHelper* GStreamerHelper::createInstance()
{
    if(!instance) {
        qDebug() << "GStreamer: stub";
    }
    return instance;
}

GStreamerHelper::GStreamerHelper()
{
}

GStreamerHelper::~GStreamerHelper()
{
    stopPlay();
}

void GStreamerHelper::getPosAndLen(long int*, long int*)
{
}

void GStreamerHelper::setPos(long int )
{
}

void GStreamerHelper::pause()
{
}

void GStreamerHelper::play()
{
}

void GStreamerHelper::startPlay(QWidget* , int , int , const QString& , int , int )
{
    stopPlay();
}

void GStreamerHelper::stopPlay()
{
}

bool GStreamerHelper::isPlay()
{
    return false;
}

QPixmap GStreamerHelper::snapshot(const QString& )
{
    QPixmap pixmap = QPixmap();
    return pixmap;
}
