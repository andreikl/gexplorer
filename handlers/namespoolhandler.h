#ifndef NAMEPOOLHANDLER_H
#define NAMEPOOLHANDLER_H

#include <QtCore/QList>

struct NamePoolData {
    QString name;
    bool isBusy;
};

class NamesPoolHandler
{
public:
    static NamesPoolHandler* createInstance();
    static NamesPoolHandler* getInstance();

private:
    NamesPoolHandler();

public:
    ~NamesPoolHandler();

public:
    NamePoolData* GetEmptyData();

private:
    static NamesPoolHandler* handler;
    QList<NamePoolData> datas;

};

#endif // NAMEPOOLHANDLER_H
