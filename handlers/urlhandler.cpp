#include "data/urldata.h"

#include "handlers/dbhandler.h"

#include "handlers/urlhandler.h"

UrlHandler* UrlHandler::handler = NULL;

UrlHandler* UrlHandler::createInstance()
{
    if(!handler) {
        UrlHandler* tempHandler = new UrlHandler();
        if(DbHandler::getInstance()->getUrls(tempHandler->urls)) {
            handler = tempHandler;
        } else {
            delete tempHandler;
        }
    }
    return handler;
}

UrlHandler* UrlHandler::getInstance()
{
    return handler;
}

UrlHandler::UrlHandler()
{
}

UrlHandler::~UrlHandler()
{
    handler = NULL;
    foreach(UrlData* url, urls) {
        delete url;
    }
}

const QList<UrlData*>& UrlHandler::getUrls() const
{
    return urls;
}

UrlData* UrlHandler::addUrl(const UrlData& value)
{
    UrlData* url = value.clone();
    if(DbHandler::getInstance()->addUrl(*url)) {
        urls.append(url);

        emit onAddUrl(url);
        return url;
    } else {
        delete url;
    }
    return NULL;
}

bool UrlHandler::delUrl(UrlData& value)
{
    if(DbHandler::getInstance()->delUrl(value)) {
        urls.removeOne(&value);

        emit onDelUrl(&value);
        delete &value;
        return true;
    }
    return false;
}

UrlData* UrlHandler::updUrl(UrlData& value)
{
    UrlData* url = getUrlById(value.getId());
    if(url->getName().compare(value.getName()) != 0) {
        if(DbHandler::getInstance()->updUrl(value)) {
            url->setName(value.getName());
            emit onUpdUrl(url);
        } else {
            url = NULL;
        }
    }
    return url;
}

UrlData* UrlHandler::getUrlById(int id) const
{
    foreach(UrlData* url, urls) {
        if(url->getId() == id) {
            return url;
        }
    }
    return NULL;
}

