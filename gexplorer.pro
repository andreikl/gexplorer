TARGET = gexplorer
TEMPLATE = app

QT += core network sql gui widgets webkit webkitwidgets
#3d

SOURCES += main.cpp \
    application.cpp \
    config.cpp \
    data/customgalleryitemdata.cpp \
    data/customgallerydata.cpp \
    data/galleryitemdata.cpp \
    data/extensiondata.cpp \
    data/gallerydata.cpp \
    data/basedata.cpp \
    data/keydata.cpp \
    data/urldata.cpp \
    helpers/commonhelper.cpp \
    handlers/customgalleryhandler.cpp \
    handlers/namespoolhandler.cpp \
    handlers/downloadhandler.cpp \
    handlers/galleryhandler.cpp \
    handlers/aliashandler.cpp \
    handlers/keyhandler.cpp \
    handlers/urlhandler.cpp \
    handlers/dbhandler.cpp \
    ui/mainwindow.cpp \
    ui/common/browsernetworkaccessmanager.cpp \
    ui/common/gallerylistdelegate.cpp \
    ui/common/galleryitemaction.cpp \
    ui/common/gallerylistmodel.cpp \
    ui/common/gallerytreemodel.cpp \
    ui/common/gallerytreeitem.cpp \
    ui/controls/customgallerycontrol.cpp \
    ui/controls/browserviewcontrol.cpp \
    ui/controls/gallerycontrol.cpp \
    ui/controls/browsercontrol.cpp \
    ui/controls/viewercontrol.cpp \
    ui/controls/titlecontrol.cpp \
    ui/controls/videocontrol.cpp \
    ui/windows/customgallerieswindow.cpp \
    ui/windows/keygallerieswindow.cpp \
    ui/windows/gallerieswindow.cpp \
    ui/windows/downloadswindow.cpp \
    ui/dialogs/editworkspacedialog.cpp \
    ui/dialogs/addgallerydialog.cpp \
    ui/dialogs/editnamedialog.cpp \
    ui/dialogs/downloaddialog.cpp \
    ui/dialogs/searchdialog.cpp \
    ui/dialogs/viewerdialog.cpp \
    ui/dialogs/unitedialog.cpp \
    ui/common/galleryitemmodel.cpp \
#    viewer3d.cpp \
#    window3d.cpp \
    data/aliasdata.cpp

HEADERS += application.h \
    config.h \
    data/customgalleryitemdata.h \
    data/customgallerydata.h \
    data/galleryitemdata.h \
    data/extensiondata.h \
    data/gallerydata.h \
    data/basedata.h \
    data/keydata.h \
    data/urldata.h \
    helpers/gstreamerhelper.h \
    helpers/commonhelper.h \
    handlers/customgalleryhandler.h \
    handlers/namespoolhandler.h \
    handlers/downloadhandler.h \
    handlers/galleryhandler.h \
    handlers/aliashandler.h \
    handlers/keyhandler.h \
    handlers/urlhandler.h \
    handlers/dbhandler.h \
    ui/mainwindow.h \
    ui/common/browsernetworkaccessmanager.h \
    ui/common/gallerylistdelegate.h \
    ui/common/galleryitemaction.h \
    ui/common/gallerylistmodel.h \
    ui/common/gallerytreemodel.h \
    ui/common/gallerytreeitem.h \
    ui/controls/customgallerycontrol.h \
    ui/controls/browserviewcontrol.h \
    ui/controls/gallerycontrol.h \
    ui/controls/browsercontrol.h \
    ui/controls/viewercontrol.h \
    ui/controls/titlecontrol.h \
    ui/controls/videocontrol.h \
    ui/windows/customgallerieswindow.h \
    ui/windows/keygallerieswindow.h \
    ui/windows/gallerieswindow.h \
    ui/windows/downloadswindow.h \
    ui/dialogs/editworkspacedialog.h \
    ui/dialogs/addgallerydialog.h \
    ui/dialogs/downloaddialog.h \
    ui/dialogs/editnamedialog.h \
    ui/dialogs/searchdialog.h \
    ui/dialogs/viewerdialog.h \
    ui/dialogs/unitedialog.h \
    ui/common/galleryitemmodel.h \
#    viewer3d.h \
#    window3d.h \
    data/aliasdata.h

FORMS += ui/mainwindow.ui \
    ui/controls/customgallerycontrol.ui \
    ui/controls/browsercontrol.ui \
    ui/controls/gallerycontrol.ui \
    ui/controls/titlecontrol.ui \
    ui/windows/customgallerieswindow.ui \
    ui/windows/keygallerieswindow.ui \
    ui/windows/gallerieswindow.ui \
    ui/windows/downloadswindow.ui \
    ui/dialogs/editworkspacedialog.ui \
    ui/dialogs/addgallerydialog.ui \
    ui/dialogs/downloaddialog.ui \
    ui/dialogs/editnamedialog.ui \
    ui/dialogs/searchdialog.ui \
    ui/dialogs/viewerdialog.ui \
    ui/dialogs/unitedialog.ui

RESOURCES += galleryexplorer.qrc

win32 {
    #INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/
    #INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/gstreamer-0.10
    #INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/glib-2.0
    #INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)lib/glib-2.0/include/
    #INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/libxml2
    #LIBS += -L$(OSSBUILD_GSTREAMER_SDK_DIR)lib
    #LIBS += -lgstinterfaces-0.10 -lgstreamer-0.10 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lglib-2.0 -lxml2 -lintl
    #INCLUDEPATH += $$PWD/win32/include/
    #INCLUDEPATH += $$PWD/win32/include/gstreamer-0.10
    #INCLUDEPATH += $$PWD/win32/include/glib-2.0
    #INCLUDEPATH += $$PWD/win32/lib/glib-2.0/include/
    #INCLUDEPATH += $$PWD/win32/include/libxml2
    #LIBS += -L$$PWD/win32/lib
    #LIBS += -lgstinterfaces-0.10 -lgstreamer-0.10 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lglib-2.0 -lxml2 -lintl
    INCLUDEPATH += $$PWD
    SOURCES += helpers/gstreamerhelper_stub.cpp
 } else {
    #PKGCONFIG += gstreamer-1.0 gstreamer-video-1.0
    #PKGCONFIG += gstreamer-1.0
    DEFINES += "GST_VERSION=0x010"
    SOURCES += helpers/gstreamerhelper.cpp

    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10 gstreamer-interfaces-0.10
    QMAKE_CXXFLAGS += -std=c++11
}

#QT += declarative

#folder_01.source = qml/GalleryExplorer
#folder_01.target = qml

#DEPLOYMENTFOLDERS += folder_01

#QML_IMPORT_PATH =

# Please do not modify the following two lines. Required for deployment.
#include(qmlapplicationviewer/qmlapplicationviewer.pri)
#qtcAddDeployment()
