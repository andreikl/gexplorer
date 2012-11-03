#include <QtCore/QDebug>

#include <QtGui/QApplication>
#include <QtGui/QWidget>

#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#include "helpers/gstreamerhelper.h"

#define CAPS "video/x-raw-rgb,bpp=(int)24,depth=(int)24"

GStreamerHelper* GStreamerHelper::instance = NULL;
GstElement* playBin = NULL;

GStreamerHelper* GStreamerHelper::getInstance()
{
    return instance;
}

GStreamerHelper* GStreamerHelper::createInstance()
{
    if(!instance) {
        gst_init(NULL, NULL);
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

void GStreamerHelper::startPlay(QWidget* area, int mw, int mh, const QString& path)
{
    stopPlay();

    //qDebug() << path;

    playBin = gst_element_factory_make("playbin2", "play");
    g_object_set(G_OBJECT(playBin), "uri", path.toAscii().constData(), NULL);

    GstElement* fakeSink = gst_element_factory_make("fakesink", NULL);
    g_object_set(G_OBJECT(playBin), "video-sink", fakeSink, NULL);

    GstState state = GST_STATE_NULL;
    gst_element_set_state (playBin, GST_STATE_PAUSED);
    gst_element_get_state (playBin, &state, NULL, GST_SECOND);

    GstPad* pad = NULL;
    GstCaps* caps = NULL;
    g_signal_emit_by_name(playBin, "get-video-pad", 0, &pad, NULL);
    caps = gst_pad_get_negotiated_caps(pad);

    int width = 0;
    int height = 0;
    gst_structure_get_int(gst_caps_get_structure(caps, 0), "width", &width);
    gst_structure_get_int(gst_caps_get_structure(caps, 0), "height", &height);
    gst_object_unref(pad);

    gst_element_set_state (playBin, GST_STATE_NULL);

    GstElement* sink = gst_element_factory_make("xvimagesink", NULL);
    g_object_set(G_OBJECT(sink), "force-aspect-ratio", TRUE, NULL);
    g_object_set(G_OBJECT(playBin), "video-sink", sink, NULL);

    area->resize(width, height);
    int x = (mw - width > 0)? (mw - width) / 2: 0;
    int y = (mh - height > 0)? (mh - height) / 2: 0;
    area->move(x, y);
    area->setVisible(true);

    //qDebug() << width << " " << height << " " << x << " " << y;

    QApplication::syncX();
#ifndef Q_OS_WIN
    gst_x_overlay_set_window_handle(GST_X_OVERLAY(sink), area->winId());
#else
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink), (gulong)area->winId());
#endif
    GstStateChangeReturn ret = gst_element_set_state(playBin, GST_STATE_PLAYING);
    if(ret == GST_STATE_CHANGE_FAILURE) {
        g_print("failed to start play\n");
        stopPlay();
    }
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
    gchar* descr = g_strdup_printf("filesrc location=%s ! decodebin2 ! ffmpegcolorspace ! appsink name=sink caps=\""CAPS"\"", path.toAscii().constData());

    //qDebug() << descr;

    GstElement* pipeline = gst_parse_launch(descr, &error);
    if(error != NULL) {
        qDebug() << "could not construct pipeline: " << error->message;
        g_error_free(error);
        res = FALSE;
    }

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

    if(res) {
        // set to PAUSED to make the first frame arrive in the sink
        GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PAUSED);
        switch (ret) {
            case GST_STATE_CHANGE_FAILURE:
                qDebug() << "failed to play the file";
                res = FALSE;
                break;

            case GST_STATE_CHANGE_NO_PREROLL:
                // for live sources, we need to set the pipeline to PLAYING before we can
                // receive a buffer. We don't do that yet
                qDebug() << "live sources not supported yet";
                res = FALSE;
                break;

            default:
                break;
        }
    }

    if(res) {
        // seek to one second
        GstStateChangeReturn ret = gst_element_get_state(pipeline, NULL, NULL, 1 * GST_SECOND);
        if(ret == GST_STATE_CHANGE_FAILURE) {
            qDebug() << "failed to seek the file";
            //res = FALSE;
        }
    }

    GstFormat format = GST_FORMAT_TIME;
    gint64 duration, position;
    if(res) {
        // get the duration
        gst_element_query_duration(pipeline, &format, &duration);

        if(duration != -1) {
            // we have a duration, seek to 5%
            position = duration * 5 / 100;
        } else {
            // no duration, seek to 1 second, this could EOS
            position = 1 * GST_SECOND;
        }

        // seek to the a position in the file. Most files have a black first frame so
        // by seeking to somewhere else we have a bigger chance of getting something
        // more interesting. An optimisation would be to detect black images and then
        // seek a little more
        res = gst_element_seek_simple(pipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH, position);
        if(!res) {
            qDebug() << "failed to seek the file";
        }
    }

    GstBuffer* buffer = NULL;
    gint width, height;
    if(res) {
        // get the preroll buffer from appsink, this block untils appsink really
        // prerolls
        g_signal_emit_by_name(sink, "pull-preroll", &buffer, NULL);

        // if we have a buffer now, convert it to a pixbuf. It's possible that we
        // don't have a buffer because we went EOS right away or had an error. */
        if(buffer) {
             GstCaps* caps;
             GstStructure* s;

             // get the snapshot buffer format now. We set the caps on the appsink so
             // that it can only be an rgb buffer. The only thing we have not specified
             // on the caps is the height, which is dependant on the pixel-aspect-ratio
             // of the source material
             caps = GST_BUFFER_CAPS(buffer);
             if(!caps) {
                 qDebug() << "could not get snapshot format";
                 res = FALSE;
             }

             if(res) {
                 s = gst_caps_get_structure(caps, 0);

                 // we need to get the final caps on the buffer to get the size
                 res = gst_structure_get_int(s, "width", &width);
                 res |= gst_structure_get_int(s, "height", &height);
                 if(!res) {
                     qDebug() << "could not get snapshot dimension";
                     res = FALSE;
                 }
             }

             if(res) {
                 // create pixmap from buffer and save, gstreamer video buffers have a stride
                 // that is rounded up to the nearest multiple of 4
                 QImage img(GST_BUFFER_DATA(buffer), width, height, GST_ROUND_UP_4(width * 3), QImage::Format_RGB888);
                 pixmap = QPixmap::fromImage(img);
                 //gdk_pixbuf_new_from_data (GST_BUFFER_DATA (buffer), GDK_COLORSPACE_RGB, FALSE, 8, width, height, GST_ROUND_UP_4 (width * 3), NULL, NULL);
             }
        }
    }

    // cleanup and exit
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return pixmap;
}
