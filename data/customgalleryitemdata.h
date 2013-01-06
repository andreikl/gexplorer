#ifndef CUSTOMGALLERYITEMDATA_H
#define CUSTOMGALLERYITEMDATA_H

#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtCore/QList>

class CustomGalleryData;
class GalleryItemData;

class CustomGalleryItemData
{
    friend class CustomGalleryHandler;
    friend class GalleryHandler;

public:
    CustomGalleryItemData(CustomGalleryData& gallery, GalleryItemData& data);
    virtual ~CustomGalleryItemData();

public:
    const QList<CustomGalleryItemData*>& getChildren() const;
    CustomGalleryData* getCustomGallery() const;
    GalleryItemData& getItem() const;

public:
    const QString& getName() const;
    int getCustomId() const;
    int getAngle() const;
    int getId() const;

public:
    void setName(const QString& value);
    void setCustomId(int value);
    void setAngle(int value);
    void setId(int value);

public:
    int width;
    int height;

public:
    CustomGalleryItemData* clone(CustomGalleryData& gallery) const;

private:
    QList<CustomGalleryItemData*> children;
    CustomGalleryData* gallery;
    GalleryItemData& item;

private:
    QString name;
    int customId;
    int angle;
    int id;
};

Q_DECLARE_METATYPE(CustomGalleryItemData*)

#endif // CUSTOMGALLERYITEMDATA_H
