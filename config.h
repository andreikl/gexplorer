#ifndef CONFIG_H
#define CONFIG_H


#include <QtCore/QString>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class Config {
public:
    enum IconSizeEnum {
        ELargeSize = 300,
        EMediumSize = 200,
        ESmallSize = 100
    };

public:
    enum FilterTypeEnum {
        EAllFilter = 0,
        ESortedFilter,
        EUnsortedFilter
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

public:
    static Config* createInstance(const QString& path);
    static Config* getInstance();

private:
    Config();

public:
    virtual ~Config();

public:
    static const char* getApplicationSignature();
    static const char* getApplicationName();

public:
    FilterTypeEnum getCustomGalleryFilterType() const;
    FilterTypeEnum getGalleryFilterType() const;
    IconSizeEnum getCustomGalleryIconSize() const;
    IconSizeEnum getGalleryIconSize() const;
    const QString& getWorkspacePath() const;
    int getActiveDownloadCount() const;

public:
    bool getIsCustomGalleriesWindow() const;
    bool getIsKeyGalleriesWindow() const;
    bool getIsGalleriesWindow() const;
    bool getIsDownloadsWindow() const;

public:
    void setCustomGalleryFilterType(FilterTypeEnum filter);
    void setGalleryFilterType(FilterTypeEnum filter);
    void setCustomGalleryIconSize(IconSizeEnum size);
    void setGalleryIconSize(IconSizeEnum size);
    void setWorkspacePath(const QString& path);
    void setActiveDownloadCount(int value);

public:
    void setIsCustomGalleriesWindow(bool value);
    void setIsKeyGalleriesWindow(bool value);
    void setIsGalleriesWindow(bool value);
    void setIsDownloadsWindow(bool value);

private:
    FilterTypeEnum customGalleryFilterType;
    FilterTypeEnum galleryFilterType;

    IconSizeEnum customGalleryIconSize;
    IconSizeEnum galleryIconSize;

    QString workspacePath;
    int activeDownloadCount;

private:
    bool isCustomGalleriesWindow;
    bool isKeyGalleriesWindow;
    bool isGalleriesWindow;
    bool isDownloadsWindow;

private:
    QSettings* handle;

private:
    static Config* instance;
};

#endif // CONFIG_H
