#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <QtCore/QObject>

class CustomGalleryData;
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
    void onUpdKey(KeyData* value);
    void onAddToKey(KeyData* value, CustomGalleryData* gallery);
    void onAddKey(KeyData* value);
    void onDelFromKey(KeyData* value, CustomGalleryData* gallery);
    void onDelKey(KeyData* value);

private:
    KeyData* getKeyById(int id) const;

private:
    static KeyHandler* handler;

private:
    QList<KeyData*> keys;
};

#endif // KEYHANDLER_H
