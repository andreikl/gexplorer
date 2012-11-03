#ifndef CUSTOMGALLERYDATA_H
#define CUSTOMGALLERYDATA_H

#include <QtCore/QMetaType>

#include "data/basedata.h"

class CustomGalleryItemData;
class KeyData;

class CustomGalleryData: public BaseData
{
    friend class CustomGalleryItemData;
    friend class KeyHandler;

public:
    CustomGalleryData();
    virtual ~CustomGalleryData();

public:
    const QList<CustomGalleryItemData*>& getItems() const;
    const QList<KeyData*>& getKeys() const;

public:
    const QString& getName() const;
    int getId() const;

public:
    void setName(const QString& value);
    void setId(int value);

public:
    CustomGalleryData* clone() const;

private:
    QList<CustomGalleryItemData*> items;
    QList<KeyData*> keys;

private:
    QString name;
    int id;

};

Q_DECLARE_METATYPE(CustomGalleryData*)

#endif // CUSTOMGALLERYDATA_H
