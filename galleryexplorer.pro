#-------------------------------------------------
#
# Project created by QtCreator 2011-04-26T15:35:19
#
#-------------------------------------------------

TARGET = galleryexplorer
TEMPLATE = app

QT += core network sql gui webkit

SOURCES += main.cpp \
    config.cpp \
    application.cpp \
    data/gallerydata.cpp \
    handlers/dbhandler.cpp \
    handlers/downloadhandler.cpp \
    ui/mainwindow.cpp \
    ui/common/gallerylistdelegate.cpp \
    ui/common/gallerylistmodel.cpp \
    ui/dialogs/editcustomgallerydialog.cpp \
    ui/dialogs/addgallerydialog.cpp \
    ui/dialogs/downloaddialog.cpp \
    ui/dialogs/editkeydialog.cpp \
    ui/dialogs/searchdialog.cpp \
    ui/controls/customgallerycontrol.cpp \
    ui/controls/browsercontrol.cpp \
    ui/controls/gallerycontrol.cpp \
    ui/controls/titlecontrol.cpp \
    ui/controls/cleancontrol.cpp \
    ui/windows/customgallerieswindow.cpp \
    ui/windows/keygallerieswindow.cpp \
    ui/windows/gallerieswindow.cpp \
    ui/windows/downloadswindow.cpp \
    helpers/commonhelper.cpp \
    ui/dialogs/viewerdialog.cpp \
    ui/common/gallerytreemodel.cpp \
    helpers/gstreamerhelper.cpp \
    ui/dialogs/unitedialog.cpp \
    handlers/keyhandler.cpp \
    handlers/galleryhandler.cpp \
    handlers/customgalleryhandler.cpp \
    data/basedata.cpp \
    data/extensiondata.cpp \
    data/customgallerydata.cpp \
    data/keydata.cpp \
    data/customgalleryitemdata.cpp \
    data/galleryitemdata.cpp \
    ui/common/gallerylistitem.cpp \
    ui/common/gallerytreeitem.cpp

HEADERS += config.h \
    application.h \
    data/gallerydata.h \
    handlers/dbhandler.h \
    handlers/downloadhandler.h \
    ui/mainwindow.h \
    ui/common/gallerylistdelegate.h \
    ui/common/gallerylistmodel.h \
    ui/dialogs/editcustomgallerydialog.h \
    ui/dialogs/addgallerydialog.h \
    ui/dialogs/downloaddialog.h \
    ui/dialogs/editkeydialog.h \
    ui/dialogs/searchdialog.h \
    ui/controls/customgallerycontrol.h \
    ui/controls/gallerycontrol.h \
    ui/controls/browsercontrol.h \
    ui/controls/titlecontrol.h \
    ui/controls/cleancontrol.h \
    ui/windows/customgallerieswindow.h \
    ui/windows/keygallerieswindow.h \
    ui/windows/gallerieswindow.h \
    ui/windows/downloadswindow.h \
    helpers/commonhelper.h \
    ui/dialogs/viewerdialog.h \
    ui/common/gallerytreemodel.h \
    helpers/gstreamerhelper.h \
    ui/dialogs/unitedialog.h \
    handlers/keyhandler.h \
    handlers/galleryhandler.h \
    handlers/customgalleryhandler.h \
    data/basedata.h \
    data/extensiondata.h \
    data/customgallerydata.h \
    data/keydata.h \
    data/customgalleryitemdata.h \
    data/galleryitemdata.h \
    ui/common/gallerylistitem.h \
    ui/common/gallerytreeitem.h

FORMS += ui/mainwindow.ui \
    ui/controls/browsercontrol.ui \
    ui/controls/gallerycontrol.ui \
    ui/controls/titlecontrol.ui \
    ui/windows/gallerieswindow.ui \
    ui/windows/downloadswindow.ui \
    ui/dialogs/searchdialog.ui \
    ui/dialogs/downloaddialog.ui \
    ui/dialogs/addgallerydialog.ui \
    ui/windows/customgallerieswindow.ui \
    ui/windows/keygallerieswindow.ui \
    ui/dialogs/editkeydialog.ui \
    ui/dialogs/editcustomgallerydialog.ui \
    ui/controls/customgallerycontrol.ui \
    ui/controls/cleancontrol.ui \
    ui/dialogs/viewerdialog.ui \
    ui/dialogs/unitedialog.ui

RESOURCES += \
    galleryexplorer.qrc

win32 {
    INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/
    INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/gstreamer-0.10
    INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/glib-2.0
    INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)lib/glib-2.0/include/
    INCLUDEPATH += $(OSSBUILD_GSTREAMER_SDK_DIR)include/libxml2
    LIBS += -L$(OSSBUILD_GSTREAMER_SDK_DIR)lib
    LIBS += -lgstinterfaces-0.10 -lgstreamer-0.10 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lglib-2.0 -lxml2 -lintl
    #INCLUDEPATH += $$PWD/win32/include/
    #INCLUDEPATH += $$PWD/win32/include/gstreamer-0.10
    #INCLUDEPATH += $$PWD/win32/include/glib-2.0
    #INCLUDEPATH += $$PWD/win32/lib/glib-2.0/include/
    #INCLUDEPATH += $$PWD/win32/include/libxml2
    #LIBS += -L$$PWD/win32/lib
    #LIBS += -lgstinterfaces-0.10 -lgstreamer-0.10 -lgmodule-2.0 -lgthread-2.0 -lgobject-2.0 -lglib-2.0 -lxml2 -lintl
 } else {
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-0.10 gstreamer-interfaces-0.10
}

QT += declarative

folder_01.source = qml/GalleryExplorer
folder_01.target = qml

DEPLOYMENTFOLDERS += folder_01

QML_IMPORT_PATH =

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
