#include "data/aliasdata.h"

#include "handlers/dbhandler.h"

#include "handlers/aliashandler.h"

AliasHandler* AliasHandler::handler = NULL;

AliasHandler* AliasHandler::createInstance()
{
    if(!handler) {
        AliasHandler* tempHandler = new AliasHandler();
        if(DbHandler::getInstance()->getAliases(tempHandler->aliases)) {
            handler = tempHandler;
        } else {
            delete tempHandler;
        }
    }
    return handler;
}

AliasHandler* AliasHandler::getInstance()
{
    return handler;
}

AliasHandler::AliasHandler()
{
}

AliasHandler::~AliasHandler()
{
    handler = NULL;
    foreach(AliasData* alias, aliases) {
        delete alias;
    }
}

const QList<AliasData*>& AliasHandler::getAliases() const
{
    return aliases;
}

AliasData* AliasHandler::getAliasByUrl(const QString& url) const
{
    foreach(AliasData* alias, aliases) {
        if(url.startsWith(alias->getAlias(), Qt::CaseInsensitive)) {
            return alias;
        }
    }
    return NULL;
}

AliasData* AliasHandler::addAlias(const AliasData& value)
{
    AliasData* alias = value.clone();
    if(DbHandler::getInstance()->addAlias(*alias)) {
        aliases.append(alias);

        emit onAddAlias(alias);
        return alias;
    } else {
        delete alias;
    }
    return NULL;
}

bool AliasHandler::delAlias(AliasData& value)
{
    if(DbHandler::getInstance()->delAlias(value)) {
        aliases.removeOne(&value);

        emit onDelAlias(&value);
        delete &value;
        return true;
    }
    return false;
}

AliasData* AliasHandler::updAlias(AliasData& value)
{
    AliasData* alias = getAliasById(value.getId());
    if(alias->getAlias().compare(value.getAlias()) != 0) {
        if(DbHandler::getInstance()->updAlias(value)) {
            alias->setAlias(value.getAlias());
            emit onUpdAlias(alias);
        } else {
            alias = NULL;
        }
    }
    return alias;
}

AliasData* AliasHandler::getAliasById(int id) const
{
    foreach(AliasData* alias, aliases) {
        if(alias->getId() == id) {
            return alias;
        }
    }
    return NULL;
}

