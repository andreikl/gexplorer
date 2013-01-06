#include <QtCore/QDebug>
#include <QtCore/QDir>

//#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>

#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#include "helpers/gstreamerhelper.h"

GStreamerHelper* GStreamerHelper::instance = NULL;

GstElement* playBin = NULL;

static gboolean bus_call(GstBus*, GstMessage* msg, gpointer)
{
    switch(GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
        qDebug() << "End-of-stream";
        GStreamerHelper::getInstance()->stopPlay();
        break;

    case GST_MESSAGE_ERROR: {
        gchar *debug = NULL;
        GError *err = NULL;
        gst_message_parse_error(msg, &err, &debug);

        qDebug() << "Error: " << err->message;
        g_error_free(err);

        if(debug) {
            qDebug() << "Debug details: " << debug;
            g_free(debug);
        }
        GStreamerHelper::getInstance()->stopPlay();
        break;
    }
    default:
        break;
    }

    return TRUE;
}

GStreamerHelper* GStreamerHelper::getInstance()
{
    return instance;
}

GStreamerHelper* GStreamerHelper::createInstance()
{
    if(!instance) {
        //int argc = QApplication::argc();
        //char** argv = QApplication::argv();
        //gst_init(&argc, &argv);
        gst_init(NULL, NULL);

        guint major, minor, micro, nano;
        gst_version(&major, &minor, &micro, &nano);
        qDebug() << "GStreamer: " << major << minor << micro << nano;

        //if(!g_thread_supported()) {
        //    g_thread_init(NULL);
        //}
        //gst_init(NULL, NULL);
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

void GStreamerHelper::getPosAndLen(long int* pos, long int* len)
{
    if(playBin) {
        GstFormat format = GST_FORMAT_TIME;
        gst_element_query_position(playBin, &format, pos);
        gst_element_query_duration(playBin, &format, len);
        //gst_element_query_position(playBin, format, pos);
        //gst_element_query_duration(playBin, format, len);
        //qDebug() << "current" << *pos;
    }
}

void GStreamerHelper::setPos(long int pos)
{
    if(playBin) {
        //qDebug() << "seek" << pos;
        gst_element_seek(playBin, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, pos, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
    }
}

void GStreamerHelper::pause()
{
    if(playBin) {
        GstStateChangeReturn ret = gst_element_set_state(playBin, GST_STATE_PAUSED);
        if(ret == GST_STATE_CHANGE_FAILURE) {
            qDebug() << "failed to pause";
        }
    }
}

void GStreamerHelper::play()
{
    if(playBin) {
        GstStateChangeReturn ret = gst_element_set_state(playBin, GST_STATE_PLAYING);
        if(ret == GST_STATE_CHANGE_FAILURE) {
            qDebug() << "failed to play";
        }
    }
}

void GStreamerHelper::startPlay(QWidget* area, int mw, int mh, const QString& path, int width, int height)
{
    stopPlay();

    GError* error = NULL;
    //gchar* descr = g_strdup_printf("filesrc location=\"%s\" ! decodebin2 ! ffmpegcolorspace ! xvimagesink name=sink", path.toAscii().constData());
    gchar* descr = g_strdup_printf("filesrc location=\"%s\" ! decodebin2 ! ffmpegcolorspace ! xvimagesink name=sink", path.toLatin1().constData());

    //qDebug() << descr;

    playBin = gst_parse_launch(descr, &error);
    if(error != NULL) {
        qDebug() << "could not construct pipeline: " << error->message;
        g_error_free(error);
    }

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(playBin));
    gst_bus_add_watch(bus, bus_call, NULL);
    gst_object_unref(bus);

    GstElement* sink = gst_bin_get_by_name(GST_BIN(playBin), "sink");

    //playBin = gst_element_factory_make("playbin2", "play");
    //g_object_set(G_OBJECT(playBin), "uri", QString("\"" + path + "\"").toAscii().constData(), NULL);

    /*GstElement* fakeSink = gst_element_factory_make("fakesink", NULL);
    g_object_set(G_OBJECT(playBin), "video-sink", fakeSink, NULL);

    GstState state = GST_STATE_NULL;
    gst_element_set_state(playBin, GST_STATE_PAUSED);
    gst_element_get_state(playBin, &state, NULL, GST_SECOND);

    GstPad* pad = NULL;
    g_signal_emit_by_name(playBin, "get-video-pad", 0, &pad, NULL);
    qDebug() << "get-video-pad compl";

    GstCaps* caps = gst_pad_get_negotiated_caps(pad);
    qDebug() << "gst_pad_get_negotiated_caps compl";*/

    /*gst_structure_get_int(gst_caps_get_structure(caps, 0), "width", &width);
    gst_structure_get_int(gst_caps_get_structure(caps, 0), "height", &height);
    gst_object_unref(pad);

    gst_element_set_state (playBin, GST_STATE_NULL);*/

    //GstElement* sink = gst_element_factory_make("xvimagesink", NULL);
    //g_object_set(G_OBJECT(sink), "force-aspect-ratio", TRUE, NULL);
    //g_object_set(G_OBJECT(playBin), "video-sink", sink, NULL);

    //QApplication::syncX();
    //gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink), (gulong)((QWidget*)((QWidget*)area->parent())->parent())->winId());
    //gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink), (gulong)((QWidget*)area->parent())->winId());
    //gst_x_overlay_set_window_handle(GST_X_OVERLAY(sink), area->winId());

    GstStateChangeReturn ret = gst_element_set_state(playBin, GST_STATE_PLAYING);
    if(ret == GST_STATE_CHANGE_FAILURE) {
        qDebug() << "failed to start play";
        stopPlay();
    }

    int x = (mw - width > 0)? (mw - width) / 2: 0;
    int y = (mh - height > 0)? (mh - height) / 2: 0;
    area->setGeometry(x, y, width, height);
    //qDebug() << x << y << width << height;

    //QApplication::syncX();
    gst_x_overlay_set_window_handle(GST_X_OVERLAY(sink), area->winId());
}

void GStreamerHelper::stopPlay()
{
    if(playBin) {
        GstStateChangeReturn ret = gst_element_set_state(playBin, GST_STATE_NULL);
        if(ret == GST_STATE_CHANGE_FAILURE) {
            g_print("failed to stop play\n");
        }
        gst_object_unref(GST_OBJECT(playBin));
        playBin = NULL;
    }
}

bool GStreamerHelper::isPlay()
{
    return playBin? true: false;
}

QPixmap GStreamerHelper::snapshot(const QString& path)
{
    QPixmap pixmap = QPixmap();
    gboolean res = TRUE;

    GError* error = NULL;
    //gchar* descr = g_strdup_printf("uridecodebin uri=\"file://%s\" ! ffmpegcolorspace ! appsink name=sink caps=\""CAPS "\"", dir.absolutePath().toAscii().constData());
    gchar* descr = g_strdup_printf("filesrc location=\"%s\" ! decodebin2 ! ffmpegcolorspace ! appsink name=sink caps=\"video/x-raw-rgb,format=RGB,bpp=(int)24,depth=(int)24\"", path.toLatin1().constData());

    //qDebug() << descr;

    GstElement* pipeline = gst_parse_launch(descr, &error);
    if(error != NULL) {
        qDebug() << "could not construct pipeline: " << error->message;
        g_error_free(error);
        res = FALSE;
    }
    //qDebug() << "parse";

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if(sink == NULL) {
        qDebug() << "could not get sink";
        res = FALSE;
    }
    //qDebug() << "get sink";

    if(res) {
        GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PAUSED);
        switch (ret) {
            case GST_STATE_CHANGE_FAILURE:
                qDebug() << "failed to paysed the file";
                res = FALSE;
                break;

            case GST_STATE_CHANGE_NO_PREROLL:
                qDebug() << "live sources not supported yet";
                res = FALSE;
                break;

            default:
                break;
        }
    }
    //qDebug() << "set pause";

    if(res) {
        GstStateChangeReturn ret = gst_element_get_state(pipeline, NULL, NULL, 1 * GST_SECOND);
        if(ret == GST_STATE_CHANGE_FAILURE) {
            qDebug() << "failed to seek the file";
            res = FALSE;
        }
    }
    //qDebug() << "seek 1 second";

    GstBuffer* buffer = NULL;
    //GstSample* sample = NULL;
    gint width, height;
    if(res) {
        g_signal_emit_by_name(sink, "pull-preroll", &buffer, NULL);
        //g_signal_emit_by_name(sink, "pull-preroll", &sample, NULL);

        //qDebug() << "preroll";

        if(buffer) {
        //if(sample) {
             GstCaps* caps;
             GstStructure* s;

             caps = GST_BUFFER_CAPS(buffer);
             //caps = gst_sample_get_caps(sample);
             if(!caps) {
                 qDebug() << "could not get snapshot format";
                 res = FALSE;
             }

             if(res) {
                 s = gst_caps_get_structure(caps, 0);

                 res = gst_structure_get_int(s, "width", &width);
                 res |= gst_structure_get_int(s, "height", &height);
                 if(!res) {
                     qDebug() << "could not get snapshot dimension";
                     res = FALSE;
                 }
             }

             if(res) {
                 QImage img(GST_BUFFER_DATA(buffer), width, height, GST_ROUND_UP_4(width * 3), QImage::Format_RGB888);
                 pixmap = QPixmap::fromImage(img);

                 /*GstMapInfo map;
                 GstBuffer* buffer = gst_sample_get_buffer(sample);
                 res = gst_buffer_map(buffer, &map, GST_MAP_READ);
                 if(res) {
                     QImage img(map.data, width, height, GST_ROUND_UP_4(width * 3), QImage::Format_RGB888);
                     pixmap = QPixmap::fromImage(img);
                     gst_buffer_unmap(buffer, &map);
                 }*/
             }
        }
    }

    //qDebug() << "end";
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return pixmap;
}
