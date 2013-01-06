#ifndef CUSTOMGALLERYHANDLER_H
#define CUSTOMGALLERYHANDLER_H

#include <QtCore/QObject>

class CustomGalleryItemData;
class CustomGalleryData;
class GalleryItemData;
class Application;

struct CustomGalleryItemName {
    CustomGalleryItemData* item;
    QString name;
};

struct CustomGalleryItemId {
    int gid;
    int id;
};

class CustomGalleryHandler: public QObject
{
    friend class GalleryHandler;
    friend class KeyHandler;
    friend class DbHandler;
    Q_OBJECT

public:
    static CustomGalleryHandler* createInstance();
    static CustomGalleryHandler* getInstance();

private:
    CustomGalleryHandler();

public:
    virtual ~CustomGalleryHandler();

public:
    const QList<CustomGalleryData*>& getCustomGalleries() const;

public:
    QList<CustomGalleryItemData*> addToCustomGallery(CustomGalleryData& value, QList<CustomGalleryItemId>& ids);

    CustomGalleryData* addCustomGallery(const CustomGalleryData& value);
    CustomGalleryData* addCustomGallery(int galleryId);

public:
    bool delFromCustomGallery(CustomGalleryItemData& value);
    bool delCustomGallery(CustomGalleryData& value);

public:
    bool updCustomGalleryItemNames(QList<CustomGalleryItemName>& names);

    bool updCustomGalleryItemUnite(CustomGalleryItemData& item, int customId);
    bool updCustomGalleryItemSplit(CustomGalleryItemData& item);

    bool updCustomGalleryItemAngle(CustomGalleryItemData& item, int angle);
    CustomGalleryData* updCustomGallery(CustomGalleryData& value);

public:
    CustomGalleryItemData* getCustomGalleryItemById(int galleryId, int id);
    CustomGalleryItemData* getCustomGalleryItemById(int id) const;
    CustomGalleryData* getCustomGalleryById(int id) const;

signals:
    void onAddCustomGalleryItem(CustomGalleryItemData* value);
    void onAddCustomGallery(CustomGalleryData* value);
    void onDelCustomGalleryItem(CustomGalleryItemData* value);
    void onDelCustomGallery(CustomGalleryData* value);

    void onUpdCustomGallery(CustomGalleryData* value, const char* name);
    void onUpdCustomGalleryItemAngle(CustomGalleryItemData* value);
    void onUpdCustomGalleryItem(CustomGalleryItemData* value);

private:
    static CustomGalleryHandler* handler;

private:
    QList<CustomGalleryData*> customGalleries;
};

#endif // CUSTOMGALLERYHANDLER_H
