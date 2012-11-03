#ifndef CUSTOMGALLERYHANDLER_H
#define CUSTOMGALLERYHANDLER_H

#include <QtCore/QObject>

class CustomGalleryItemData;
class CustomGalleryData;
class GalleryItemData;
class Application;

class CustomGalleryHandler: public QObject
{
    friend class GalleryHandler;
    friend class KeyHandler;
    friend class DbHandler;
    Q_OBJECT

public:
    static CustomGalleryHandler* createInstance(Application* app);
    static CustomGalleryHandler* getInstance();

private:
    CustomGalleryHandler();

public:
    virtual ~CustomGalleryHandler();

public:
    const QList<CustomGalleryData*>& getCustomGalleries() const;

public:
    CustomGalleryItemData* addToCustomGallery(CustomGalleryData& value, int galleryId, int itemId);
    CustomGalleryData* addCustomGallery(const CustomGalleryData& value);
    CustomGalleryData* addCustomGallery(int galleryId);

public:
    bool delFromCustomGallery(CustomGalleryItemData& value);
    bool delCustomGallery(CustomGalleryData& value);

public:
    bool updCustomGalleryItemName(CustomGalleryItemData& item, const QString& name);
    bool updCustomGalleryItemCustomId(CustomGalleryItemData& item, int customId);
    bool updCustomGalleryItemAngle(CustomGalleryItemData& item, int angle);
    CustomGalleryData* updCustomGallery(CustomGalleryData& value);

signals:
    void onAddCustomGalleryItem(CustomGalleryItemData* pValue);
    void onAddCustomGallery(CustomGalleryData* pValue);
    void onDelCustomGalleryItem(CustomGalleryItemData* pValue);
    void onDelCustomGallery(CustomGalleryData* pValue);
    void onUpdCustomGalleryItemAngle(CustomGalleryItemData* pValue);
    void onUpdCustomGalleryItem(CustomGalleryItemData* pValue);
    void onUpdCustomGallery(CustomGalleryData* pValue);

private:
    CustomGalleryItemData* getCustomGalleryItemById(int galleryId, int id) const;
    CustomGalleryItemData* getCustomGalleryItemById(int id) const;
    CustomGalleryData* getCustomGalleryById(int id) const;

private:
    static CustomGalleryHandler* handler;

private:
    QList<CustomGalleryData*> customGalleries;
};

#endif // CUSTOMGALLERYHANDLER_H
