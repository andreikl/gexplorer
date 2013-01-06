#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <QtCore/QObject>

class CustomGalleryData;
class Application;
class KeyData;

class KeyHandler: public QObject
{
    friend class CustomGalleryHandler;
    Q_OBJECT

public:
    static KeyHandler* createInstance();
    static KeyHandler* getInstance();

private:
    KeyHandler();

public:
    virtual ~KeyHandler();

public:
    const QList<KeyData*>& getKeys() const;

public:
    CustomGalleryData* addToKey(KeyData& key, int galleryId);
    KeyData* addKey(const KeyData& value);

public:
    bool delFromKey(KeyData& key, CustomGalleryData& value);
    bool delKey(KeyData& value);

public:
    KeyData* updKey(KeyData& value);

signals:
    void onUpdKey(KeyData* pValue);
    void onAddToKey(KeyData* pValue, CustomGalleryData* gallery);
    void onAddKey(KeyData* pValue);
    void onDelFromKey(KeyData* pValue, CustomGalleryData* gallery);
    void onDelKey(KeyData* pValue);

private:
    KeyData* getKeyById(int id) const;

private:
    static KeyHandler* handler;

private:
    QList<KeyData*> keys;
};

#endif // KEYHANDLER_H
