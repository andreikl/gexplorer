#ifndef GALLERYITEMDATA_H
#define GALLERYITEMDATA_H

#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtCore/QList>

class CustomGalleryItemData;
class ExtensionData;
class GalleryData;

class GalleryItemData
{
    friend class CustomGalleryHandler;
    friend class GalleryHandler;

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
    GalleryData* getGallery() const;

public:
    GalleryItemStatusEnum getStatus() const;
    const QString& getFileName() const;
    const QString& getPath() const;
    const QString& getUrl() const;
    int getId() const;

    GalleryItemData* getReference() const;
    int getReferenceAngle() const;

public:
    void setStatus(GalleryItemStatusEnum value);
    void setFileName(const QString& value);
    void setPath(const QString& value);
    void setUrl(const QString& value);
    void setId(int value);

    void setReference(GalleryItemData* reference);
    void setReferenceAngle(int angle);

public:
    int width;
    int height;

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

    GalleryItemData* reference;
    int referenceAngle;
};

Q_DECLARE_METATYPE(GalleryItemData*)

#endif // GALLERYITEMDATA_H
