#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QtSql/QSqlDatabase>

struct CustomGalleryItemName;
class CustomGalleryItemData;
class CustomGalleryHandler;
class CustomGalleryData;
class GalleryItemData;
class GalleryHandler;
class ExtensionData;
class GalleryData;
class Application;
class AliasData;
class KeyData;
class UrlData;

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
    bool getGalleries(QList<GalleryData*>& galleries);
    bool getGalleryItems(Application* app, GalleryHandler* handler);

    bool getCustomGalleries(QList<CustomGalleryData*>& customGalleries);
    bool getCustomGalleryItems(CustomGalleryHandler* handler);

    bool getExtensions(QList<ExtensionData*>& extensions);
    bool getKeys(QList<KeyData*>& keys);
    bool getUrls(QList<UrlData*>& urls);
    bool getAliases(QList<AliasData*>& aliases);

public:
    bool addToKey(const KeyData& key, const CustomGalleryData& value);
    bool addToCustomGallery(QList<CustomGalleryItemData*>& items);
    bool addCustomGallery(CustomGalleryData& value);
    bool addToGallery(GalleryData& value);
    bool addGallery(GalleryData& value);
    bool addKey(KeyData& value);
    bool addUrl(UrlData& value);
    bool addAlias(AliasData& value);

public:
    bool delFromKey(const KeyData& key, const CustomGalleryData& value);
    bool delFromCustomGallery(const CustomGalleryItemData& value);
    bool delCustomGallery(const CustomGalleryData& value);
    bool delFromGallery(const GalleryItemData& value);
    bool delGallery(const GalleryData& value);
    bool delKey(const KeyData& value);
    bool delUrl(const UrlData& value);
    bool delAlias(const AliasData& value);

public:
    bool updCustomGalleryItemName(const CustomGalleryItemData& item, const QString& name);
    bool updCustomGalleryItemNames(QList<CustomGalleryItemName>& names);

    bool updCustomGalleryItemUnite(const CustomGalleryItemData& item, int customId);
    bool updCustomGalleryItemSplit(const CustomGalleryItemData& item);

    bool updCustomGalleryItemAngle(const CustomGalleryItemData& item, int angle);
    bool updCustomGallery(const CustomGalleryData& value);

    bool updGalleryItemReference(const GalleryItemData& item, const GalleryItemData& reference, int angle);
    bool updGalleryItemStatus(const GalleryItemData& item);
    bool updGallerySource(const GalleryData& value);

    bool updKey(const KeyData& value);
    bool updUrl(const UrlData& value);
    bool updAlias(const AliasData& value);

private:
    static void checkResAndWriteError(bool res, const QString& error, const QString& text = "");

private:
    static DbHandler* handler;

private:
    QSqlDatabase db;
};

#endif // DBHANDLER_H
