#ifndef GALLERY_DATA_H
#define GALLERY_DATA_H

#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtCore/QList>

#include "data/basedata.h"

class GalleryItemData;

class GalleryData: public BaseData
{
    friend class GalleryItemData;

public:
    enum GalleryTypeEnum {
        WebGallery = 1,
        FileGallery,
        DeletedGallery
    };

public:
    GalleryData(GalleryTypeEnum type);
    virtual ~GalleryData();

public:
    const QList<GalleryItemData*>& getItems() const;
    GalleryTypeEnum getType() const;

public:
    const QString& getSource() const;
    const QString& getPath() const;
    int getId() const;

public:
    void setSource(const QString& value);
    void setPath(const QString& value);
    void setId(int value);

public:
    bool isAllContains() const;
    GalleryData* clone(bool isItems = true) const;

private:
    QList<GalleryItemData*> items;
    GalleryTypeEnum type;

private:
    QString source;
    QString path;
    int id;
};

Q_DECLARE_METATYPE(GalleryData*)

#endif // GALLERY_DATA_H
