#ifndef ALIASHANDLER_H
#define ALIASHANDLER_H

#include <QtCore/QObject>

class AliasData;

class AliasHandler: public QObject
{
    Q_OBJECT

public:
    static AliasHandler* createInstance();
    static AliasHandler* getInstance();

private:
    AliasHandler();

public:
    virtual ~AliasHandler();

public:
    const QList<AliasData*>& getAliases() const;
    AliasData* getAliasByUrl(const QString& url) const;

public:
    AliasData* addAlias(const AliasData& value);

    bool delAlias(AliasData& value);

    AliasData* updAlias(AliasData& value);

signals:
    void onUpdAlias(AliasData* value);
    void onAddAlias(AliasData* value);
    void onDelAlias(AliasData* value);

private:
    AliasData* getAliasById(int id) const;

private:
    static AliasHandler* handler;

private:
    QList<AliasData*> aliases;
};

#endif // ALIASHANDLER_H
