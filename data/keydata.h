#ifndef KEYDATA_H
#define KEYDATA_H

#include <QtCore/QMetaType>

#include "data/basedata.h"

class CustomGalleryData;

class KeyData: public BaseData
{
    friend class CustomGalleryHandler;
    friend class KeyHandler;
    friend class DbHandler;

public:
    KeyData();
    virtual ~KeyData();

public:
    const QList<CustomGalleryData*>& getGalleries() const;

public:
    const QString& getName() const;
    int getId() const;

public:
    void setName(const QString& value);
    void setId(int value);

public:
    bool isContainGallery(int id) const;
    KeyData* clone() const;

private:
    QList<CustomGalleryData*> galleries;

private:
    QString name;
    int id;
};

Q_DECLARE_METATYPE(KeyData*)

#endif // KEYDATA_H
