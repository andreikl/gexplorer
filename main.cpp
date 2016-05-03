#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <QtWidgets/QApplication>

#include <QtWebKit/QWebSettings>

#include "application.h"
#include "config.h"

#include "helpers/commonhelper.h"
#include "helpers/gstreamerhelper.h"

#include "ui/dialogs/editworkspacedialog.h"

#include "ui/mainwindow.h"

/*#include <QtCore/QThread>
#include <gst/gst.h>
class MyThread : public QThread
{
 protected:
     void run();
 };

 void MyThread::run()
 {
     gst_init(NULL, NULL);

     guint major, minor, micro, nano;
     gst_version(&major, &minor, &micro, &nano);
     qDebug() << "GStreamer: " << major << minor << micro << nano;

     QString p = "file:///home/tiamat/workspace/1/1.avi";
     qDebug() << p;

     //GstElement* pipeline = gst_pipeline_new("pipeline");
     GstElement* pipeline = gst_element_factory_make("playbin", NULL);
     g_object_set(pipeline, "uri", p.toAscii().constData(), NULL);

     //GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
     //guint bus_watch_id = gst_bus_add_watch(bus, bus_call, NULL);
     //gst_object_unref(bus);

     GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
     if(ret == GST_STATE_CHANGE_FAILURE) {
         qDebug() << "Gstreamer: failed to start play";
     }
 }*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int res = 0;

    //MyThread* thr = new MyThread();
    //thr->start();

    QWebSettings* ws =  QWebSettings::globalSettings();
    ws->setAttribute(QWebSettings::PluginsEnabled, true);
    ws->setAttribute(QWebSettings::JavascriptEnabled, true);
    ws->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    ws->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);

    Config::createInstance(QDir::currentPath());
    GStreamerHelper::createInstance();

    QString workspace = Config::getInstance()->getWorkspacePath();
    if(workspace.length() == 0 || !QDir(workspace).exists()) {
        EditWorkspaceDialog* dialog = new EditWorkspaceDialog(NULL);
        dialog->show();
        dialog->exec();

        if(dialog->result() == QDialog::Accepted) {
            Config::getInstance()->setWorkspacePath(workspace = dialog->getWorkspace());
        }
        delete dialog;
    }

    if(workspace.length() > 0 && QDir(workspace).exists()) {
        QDir::setCurrent(workspace);

        if(Application::createInstance(workspace)) {
            MainWindow w(NULL);
            w.showMaximized();

            res = a.exec();
        }
    }

    if(Application::getInstance()) {
        delete Application::getInstance();
    }
    if(GStreamerHelper::getInstance()) {
        delete GStreamerHelper::getInstance();
    }
    if(Config::getInstance()) {
        delete Config::getInstance();
    }
    return res;
}
