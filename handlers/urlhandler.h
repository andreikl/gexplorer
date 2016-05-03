#ifndef URLHANDLER_H
#define URLHANDLER_H

#include <QtCore/QObject>

class UrlData;

class UrlHandler: public QObject
{
    Q_OBJECT

public:
    static UrlHandler* createInstance();
    static UrlHandler* getInstance();

private:
    UrlHandler();

public:
    virtual ~UrlHandler();

public:
    const QList<UrlData*>& getUrls() const;

public:
    UrlData* addUrl(const UrlData& value);

    bool delUrl(UrlData& value);

    UrlData* updUrl(UrlData& value);

signals:
    void onUpdUrl(UrlData* value);
    void onAddUrl(UrlData* value);
    void onDelUrl(UrlData* value);

private:
    UrlData* getUrlById(int id) const;

private:
    static UrlHandler* handler;

private:
    QList<UrlData*> urls;
};

#endif // URLHANDLER_H
