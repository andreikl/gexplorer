#include <QtCore/QDir>

#include <QtGui/QApplication>

#include "helpers/commonhelper.h"
#include "helpers/gstreamerhelper.h"

#include "ui/mainwindow.h"

#include "config.h"

#include "application.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    GStreamerHelper::createInstance();
    Config::Create(QDir::currentPath());

    int res = 0;
    if(Application::createInstance(QDir::currentPath())) {
        MainWindow w;
        w.showMaximized();

        res = a.exec();
    }

    if(Application::getInstance()) {
        delete Application::getInstance();
    }
    if(GStreamerHelper::getInstance()) {
        delete GStreamerHelper::getInstance();
    }
    Config::Delete();

    return res;
}
