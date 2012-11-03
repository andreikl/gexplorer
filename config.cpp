#define CONFIG_FILE_NAME "config.ini"

// all
#define APPLICATION_TAG "GalleryExplorer"
#define APPLICATION_NAME "Gallery Explorer"
#define APPLICATION_SIGNATURE "application/GalleryExplorer-Gallery"
// download
#define ACTIVE_DOWNLOAD_COUNT_TAG "ActiveDownloadCount"
// ui
#define GALLERY_ICON_SIZE_TAG "GalleryIconSize"
#define CUSTOM_GALLERY_ICON_SIZE_TAG "CustomGalleryIconSize"
// ui
#define IS_KEY_GALLERIES_WINDOW_TAG "IsKeyGalleriesWindow"
#define IS_CUSTOM_GALLERIES_WINDOW_TAG "IsCustomGalleriesWindow"
#define IS_GALLERIES_WINDOW_TAG "IsGalleriesWindow"
#define IS_DOWNLOADS_WINDOW_TAG "IsDownloadsWindow"

#include <QtCore/QDir>

#include "config.h"

Config* Config::pConfig = NULL;

Config::Config(const QString& path): pHandle(NULL) {

    //QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, path);
    //QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, path);
    //QSettings::setSystemIniPath(path);
    QSettings::setUserIniPath(path + QDir::separator());
    pHandle = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);

    pHandle->beginGroup(APPLICATION_TAG);
    // common
    // download
    activeDownloadCount = pHandle->value(ACTIVE_DOWNLOAD_COUNT_TAG, 3).toInt();
    // ui
    galleryIconSize = (IconSizeEnum)pHandle->value(GALLERY_ICON_SIZE_TAG, EMediumSize).toInt();
    customGalleryIconSize = (IconSizeEnum)pHandle->value(CUSTOM_GALLERY_ICON_SIZE_TAG, EMediumSize).toInt();
    // ui
    isKeyGalleriesWindow = pHandle->value(IS_KEY_GALLERIES_WINDOW_TAG, true).toBool();
    isCustomGalleriesWindow = pHandle->value(IS_CUSTOM_GALLERIES_WINDOW_TAG, true).toBool();
    isGalleriesWindow = pHandle->value(IS_GALLERIES_WINDOW_TAG, true).toBool();
    isDownloadsWindow = pHandle->value(IS_DOWNLOADS_WINDOW_TAG, true).toBool();
    pHandle->endGroup();
}

Config::~Config() {
    if(pHandle) {
        delete pHandle;
        pHandle = NULL;
    }
}

Config* Config::Self() {
    return pConfig;
}

Config* Config::Create(const QString& path) {
    if(!pConfig) {
        pConfig= new Config(path);
    }
    return pConfig;
}

void Config::Delete() {
    if(pConfig) {
        delete pConfig;
        pConfig = NULL;
    }
}

const char* Config::ApplicationName() {
    return APPLICATION_TAG;
}

const char* Config::ApplicationSignature() {
    return APPLICATION_SIGNATURE;
}

int Config::ActiveDownloadCount() const {
    return activeDownloadCount;
}

void Config::ActiveDownloadCount(int value) {
    this->activeDownloadCount = value;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(ACTIVE_DOWNLOAD_COUNT_TAG, activeDownloadCount);
    pHandle->endGroup();
}

Config::IconSizeEnum Config::GalleryIconSize() const {
    return galleryIconSize;
}

void Config::GalleryIconSize(IconSizeEnum size) {
    this->galleryIconSize = size;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(GALLERY_ICON_SIZE_TAG, galleryIconSize);
    pHandle->endGroup();
}

Config::IconSizeEnum Config::CustomGalleryIconSize() const {
    return customGalleryIconSize;
}

void Config::CustomGalleryIconSize(IconSizeEnum size) {
    this->customGalleryIconSize = size;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(CUSTOM_GALLERY_ICON_SIZE_TAG, customGalleryIconSize);
    pHandle->endGroup();
}

bool Config::IsKeyGalleriesWindow() const {
    return isKeyGalleriesWindow;
}

void Config::IsKeyGalleriesWindow(bool value) {
    this->isKeyGalleriesWindow = value;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(IS_KEY_GALLERIES_WINDOW_TAG, isKeyGalleriesWindow);
    pHandle->endGroup();
}

bool Config::IsCustomGalleriesWindow() const {
    return isCustomGalleriesWindow;
}

void Config::IsCustomGalleriesWindow(bool value) {
    this->isCustomGalleriesWindow = value;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(IS_CUSTOM_GALLERIES_WINDOW_TAG, isCustomGalleriesWindow);
    pHandle->endGroup();
}

bool Config::IsGalleriesWindow() const {
    return isGalleriesWindow;
}

void Config::IsGalleriesWindow(bool value) {
    this->isGalleriesWindow = value;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(IS_GALLERIES_WINDOW_TAG, isGalleriesWindow);
    pHandle->endGroup();
}

bool Config::IsDownloadsWindow() const {
    return isDownloadsWindow;
}

void Config::IsDownloadsWindow(bool value) {
    this->isDownloadsWindow = value;

    pHandle->beginGroup(APPLICATION_TAG);
    pHandle->setValue(IS_DOWNLOADS_WINDOW_TAG, isDownloadsWindow);
    pHandle->endGroup();
}
