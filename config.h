#ifndef CONFIG_H
#define CONFIG_H

#define APP_NAME "Gallery Explorer"

#include <QtCore/QSettings>
#include <QtCore/QStringList>

class Config {
public:
    enum IconSizeEnum {
        ELargeSize = 300,
        EMediumSize = 200,
        ESmallSize = 100
    };

public:
    enum ItemTypeEnum {
        ETypeUnknown = 0,
        ETypeGallery,
        ETypeGalleryItem,
        ETypeCustomGallery,
        ETypeCustomGalleryItem,
        ETypeKey
    };

private:
    Config(const QString& path);

public:
    virtual ~Config();

public:
    static Config* Self();
    static Config* Create(const QString& path);
    static void Delete();

public:
    static const char* ApplicationName();
    static const char* ApplicationSignature();

// common
// download
public:
    int ActiveDownloadCount() const;
    void ActiveDownloadCount(int value);

// ui
public:
    IconSizeEnum GalleryIconSize() const;
    void GalleryIconSize(IconSizeEnum size);
    IconSizeEnum CustomGalleryIconSize() const;
    void CustomGalleryIconSize(IconSizeEnum size);

// ui
public:
    bool IsKeyGalleriesWindow() const;
    void IsKeyGalleriesWindow(bool value);
    bool IsCustomGalleriesWindow() const;
    void IsCustomGalleriesWindow(bool value);
    bool IsGalleriesWindow() const;
    void IsGalleriesWindow(bool value);
    bool IsDownloadsWindow() const;
    void IsDownloadsWindow(bool value);

// common
// download
private:
    int activeDownloadCount;

// ui
private:
    IconSizeEnum galleryIconSize;
    IconSizeEnum customGalleryIconSize;

// ui
private:
    bool isKeyGalleriesWindow;
    bool isCustomGalleriesWindow;
    bool isGalleriesWindow;
    bool isDownloadsWindow;

private:
    static Config* pConfig;
    QSettings* pHandle;

};

#endif // CONFIG_H
