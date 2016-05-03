#include "data/aliasdata.h"

AliasData::AliasData()
{
}

AliasData::~AliasData()
{
}

int AliasData::getId() const
{
    return id;
}

const QString& AliasData::getAlias() const
{
    return alias;
}

void AliasData::setId(int value)
{
    id = value;
}

void AliasData::setAlias(const QString& value)
{
    alias = value;
}

AliasData* AliasData::clone() const
{
    AliasData* a = new AliasData();
    a->id = id;
    a->alias = alias;
    return a;
}
