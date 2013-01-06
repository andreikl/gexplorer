#include "data/extensiondata.h"

ExtensionData::ExtensionData(int id, const QString& name, ExtensionTypeEnum type)
{
    this->id = id;
    this->name = name;
    this->type = type;
}

ExtensionData::~ExtensionData()
{
}

ExtensionData::ExtensionTypeEnum ExtensionData::getType() const
{
    return type;
}

const QString& ExtensionData::getName() const {
    return name;
}

int ExtensionData::getId() const
{
    return id;
}
