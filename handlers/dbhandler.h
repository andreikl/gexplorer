#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtSql/QSqlDatabase>

class CustomGalleryItemData;
class CustomGalleryData;
class GalleryItemData;
class ExtensionData;
class GalleryData;
class Application;
class KeyData;

class DbHandler
{
public:
    static DbHandler* createInstance(const QString& path);
    static DbHandler* getInstance();

private:
    DbHandler();

public:
    virtual ~DbHandler();

public:
    bool Create(const QString& path);

public:
    bool getGalleries(Application* app, QList<GalleryData*>& galleries);
    bool getCustomGalleries(QList<CustomGalleryData*>& customGalleries);
    bool getExtensions(QList<ExtensionData*>& extensions);
    bool getKeys(QList<KeyData*>& keys);

public:
    bool addToKey(const KeyData& key, const CustomGalleryData& value);
    bool addToCustomGallery(CustomGalleryItemData& value);
    bool addCustomGallery(CustomGalleryData& value);
    bool addGallery(GalleryData& value);
    bool addKey(KeyData& value);

public:
    bool delFromKey(const KeyData& key, const CustomGalleryData& value);
    bool delFromCustomGallery(const CustomGalleryItemData& value);
    bool delCustomGallery(const CustomGalleryData& value);
    bool delGallery(const GalleryData& value);
    bool delKey(const KeyData& value);

public:
    bool updCustomGalleryItemName(const CustomGalleryItemData& item, const QString& name);
    bool updCustomGalleryItemCustomId(const CustomGalleryItemData& item, int customId);
    bool updCustomGalleryItemAngle(const CustomGalleryItemData& item, int angle);
    bool updCustomGallery(const CustomGalleryData& value);
    bool updGalleryItemStatus(const GalleryItemData& item);
    bool updKey(const KeyData& value);

private:
    static void checkResAndWriteError(bool res, const QString& error, const QString& text = "");

private:
    static DbHandler* handler;

private:
    QSqlDatabase db;
};

#endif // DBHANDLER_H
