#ifndef GSTREAMERHELPER_H
#define GSTREAMERHELPER_H

#include <QtGui/QPixmap>

class GStreamerHelper
{
public:
    static GStreamerHelper* createInstance();
    static GStreamerHelper* getInstance();

private:
    GStreamerHelper();

public:
    virtual ~GStreamerHelper();

public:
    void startPlay(QWidget* area, int mw, int mh, const QString& path);
    void stopPlay();
    bool isPlay();

public:
    static QPixmap snapshot(const QString& url);

private:
    static GStreamerHelper* instance;
};

#endif // GSTREAMERHELPER_H
