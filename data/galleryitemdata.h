#ifndef GALLERYITEMDATA_H
#define GALLERYITEMDATA_H

#include <QtCore/QMetaType>

class CustomGalleryItemData;
class ExtensionData;
class GalleryData;

class GalleryItemData
{
    friend class CustomGalleryHandler;

public:
    enum GalleryItemStatusEnum {
        StatusNew = 1,
        StatusDownload,
        StatusComplete,
        StatusError
    };

public:
    GalleryItemData(GalleryData* gallery, const ExtensionData& extension);
    virtual ~GalleryItemData();

public:
    const QList<CustomGalleryItemData*>& getCustomItems() const;
    const ExtensionData& getExtension() const;
    const GalleryData* getGallery() const;

public:
    GalleryItemStatusEnum getStatus() const;
    const QString& getFileName() const;
    const QString& getPath() const;
    const QString& getUrl() const;
    int getId() const;

public:
    void setStatus(GalleryItemStatusEnum value);
    void setFileName(const QString& value);
    void setPath(const QString& value);
    void setUrl(const QString& value);
    void setId(int value);

public:
    GalleryItemData* clone(GalleryData* gallery) const;

private:
    QList<CustomGalleryItemData*> customItems;
    const ExtensionData* extension;
    GalleryData* gallery;

private:
    GalleryItemStatusEnum status;
    QString fileName;
    QString path;
    QString url;
    int id;

};

Q_DECLARE_METATYPE(GalleryItemData*)

#endif // GALLERYITEMDATA_H
