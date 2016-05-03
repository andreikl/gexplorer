#define CONFIG_FILE_NAME "config.ini"

#define APPLICATION_SIGNATURE "application/GalleryExplorer-Gallery"
#define APPLICATION_NAME "GalleryExplorer"

#define CUSTOM_GALLERY_FILTER_TYPE_TAG "CustomGalleryFiterType"
#define GALLERY_FILTER_TYPE_TAG "GalleryFiterType"

#define CUSTOM_GALLERY_ICON_SIZE_TAG "CustomGalleryIconSize"
#define GALLERY_ICON_SIZE_TAG "GalleryIconSize"

#define ACTIVE_DOWNLOAD_COUNT_TAG "ActiveDownloadCount"
#define WORKSPACE_PATH_TAG "WorkspacePathTag"

#define IS_CUSTOM_GALLERIES_WINDOW_TAG "IsCustomGalleriesWindow"
#define IS_KEY_GALLERIES_WINDOW_TAG "IsKeyGalleriesWindow"
#define IS_GALLERIES_WINDOW_TAG "IsGalleriesWindow"
#define IS_DOWNLOADS_WINDOW_TAG "IsDownloadsWindow"

#include <QtCore/QSettings>
#include <QtCore/QDir>

#include "config.h"

Config* Config::instance = NULL;

Config* Config::getInstance()
{
    return instance;
}

Config* Config::createInstance(const QString& path)
{
    if(!instance) {
        QSettings::setUserIniPath(path + QDir::separator());
        Config* tempInstance = new Config();

        tempInstance->handle = new QSettings(CONFIG_FILE_NAME, QSettings::IniFormat);

        tempInstance->handle->beginGroup(APPLICATION_NAME);

        tempInstance->galleryFilterType = (FilterTypeEnum)tempInstance->handle->value(GALLERY_FILTER_TYPE_TAG, EAllFilter).toInt();
        tempInstance->customGalleryFilterType = (FilterTypeEnum)tempInstance->handle->value(CUSTOM_GALLERY_FILTER_TYPE_TAG, EAllFilter).toInt();

        tempInstance->customGalleryIconSize = (IconSizeEnum)tempInstance->handle->value(CUSTOM_GALLERY_ICON_SIZE_TAG, EMediumSize).toInt();
        tempInstance->galleryIconSize = (IconSizeEnum)tempInstance->handle->value(GALLERY_ICON_SIZE_TAG, EMediumSize).toInt();

        tempInstance->workspacePath = tempInstance->handle->value(WORKSPACE_PATH_TAG, "").toString();
        tempInstance->activeDownloadCount = tempInstance->handle->value(ACTIVE_DOWNLOAD_COUNT_TAG, 5).toInt();

        tempInstance->isCustomGalleriesWindow = tempInstance->handle->value(IS_CUSTOM_GALLERIES_WINDOW_TAG, true).toBool();
        tempInstance->isKeyGalleriesWindow = tempInstance->handle->value(IS_KEY_GALLERIES_WINDOW_TAG, true).toBool();
        tempInstance->isGalleriesWindow = tempInstance->handle->value(IS_GALLERIES_WINDOW_TAG, true).toBool();
        tempInstance->isDownloadsWindow = tempInstance->handle->value(IS_DOWNLOADS_WINDOW_TAG, true).toBool();

        tempInstance->handle->endGroup();

        instance = tempInstance;
    }
    return instance;
}

Config::Config()
{
}

Config::~Config()
{
    if(handle) {
        delete handle;
        handle = NULL;
    }
}

const char* Config::getApplicationSignature()
{
    return APPLICATION_SIGNATURE;
}

const char* Config::getApplicationName()
{
    return APPLICATION_NAME;
}

Config::FilterTypeEnum Config::getCustomGalleryFilterType() const
{
    return customGalleryFilterType;
}

Config::FilterTypeEnum Config::getGalleryFilterType() const
{
    return galleryFilterType;
}

Config::IconSizeEnum Config::getCustomGalleryIconSize() const
{
    return customGalleryIconSize;
}

Config::IconSizeEnum Config::getGalleryIconSize() const
{
    return galleryIconSize;
}

const QString& Config::getWorkspacePath() const
{
    return workspacePath;
}

int Config::getActiveDownloadCount() const
{
    return activeDownloadCount;
}

bool Config::getIsCustomGalleriesWindow() const
{
    return isCustomGalleriesWindow;
}

bool Config::getIsKeyGalleriesWindow() const
{
    return isKeyGalleriesWindow;
}

bool Config::getIsGalleriesWindow() const
{
    return isGalleriesWindow;
}

bool Config::getIsDownloadsWindow() const
{
    return isDownloadsWindow;
}

void Config::setCustomGalleryFilterType(FilterTypeEnum filter)
{
    this->customGalleryFilterType = filter;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(CUSTOM_GALLERY_FILTER_TYPE_TAG, customGalleryFilterType);
    handle->endGroup();
}

void Config::setGalleryFilterType(FilterTypeEnum filter)
{
    this->galleryFilterType = filter;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(GALLERY_FILTER_TYPE_TAG, galleryFilterType);
    handle->endGroup();
}

void Config::setCustomGalleryIconSize(IconSizeEnum size)
{
    this->customGalleryIconSize = size;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(CUSTOM_GALLERY_ICON_SIZE_TAG, customGalleryIconSize);
    handle->endGroup();
}

void Config::setGalleryIconSize(IconSizeEnum size)
{
    this->galleryIconSize = size;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(GALLERY_ICON_SIZE_TAG, galleryIconSize);
    handle->endGroup();
}

void Config::setWorkspacePath(const QString& path)
{
    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(WORKSPACE_PATH_TAG, workspacePath = path);
    handle->endGroup();
}

void Config::setActiveDownloadCount(int value)
{
    this->activeDownloadCount = value;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(ACTIVE_DOWNLOAD_COUNT_TAG, activeDownloadCount);
    handle->endGroup();
}

void Config::setIsCustomGalleriesWindow(bool value)
{
    this->isCustomGalleriesWindow = value;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(IS_CUSTOM_GALLERIES_WINDOW_TAG, isCustomGalleriesWindow);
    handle->endGroup();
}

void Config::setIsKeyGalleriesWindow(bool value)
{
    this->isKeyGalleriesWindow = value;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(IS_KEY_GALLERIES_WINDOW_TAG, isKeyGalleriesWindow);
    handle->endGroup();
}

void Config::setIsGalleriesWindow(bool value)
{
    this->isGalleriesWindow = value;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(IS_GALLERIES_WINDOW_TAG, isGalleriesWindow);
    handle->endGroup();
}

void Config::setIsDownloadsWindow(bool value)
{
    this->isDownloadsWindow = value;

    handle->beginGroup(APPLICATION_NAME);
    handle->setValue(IS_DOWNLOADS_WINDOW_TAG, isDownloadsWindow);
    handle->endGroup();
}
