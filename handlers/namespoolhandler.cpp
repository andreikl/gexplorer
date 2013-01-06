#include "config.h"

#include "handlers/namespoolhandler.h"

NamesPoolHandler* NamesPoolHandler::handler = NULL;

NamesPoolHandler* NamesPoolHandler::createInstance()
{
    if(!handler) {
        handler = new NamesPoolHandler();
        for(int i = 0; i < Config::getInstance()->getActiveDownloadCount(); i++) {
            NamePoolData data;
            data.name = Config::getInstance()->getApplicationName() + QString::number(i) + ".bak";
            data.isBusy = false;
            handler->datas.append(data);

        }
    }
    return handler;
}

NamesPoolHandler* NamesPoolHandler::getInstance()
{
    return handler;
}

NamesPoolHandler::~NamesPoolHandler()
{
    handler = NULL;
}

NamesPoolHandler::NamesPoolHandler()
{
}

NamePoolData* NamesPoolHandler::GetEmptyData()
{
    for(int i = 0; i < datas.length(); i++) {
        if(!datas[i].isBusy) {
            return &datas[i];
        }
    }
    return NULL;
}
