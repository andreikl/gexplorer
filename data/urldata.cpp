#include "data/urldata.h"

UrlData::UrlData()
{
}

UrlData::~UrlData()
{
}

int UrlData::getId() const
{
    return id;
}

const QString& UrlData::getName() const
{
    return name;
}

const QString& UrlData::getSource() const
{
    return source;
}

void UrlData::setId(int value)
{
    id = value;
}

void UrlData::setName(const QString& value)
{
    name = value;
}

void UrlData::setSource(const QString& value)
{
    source = value;
}

UrlData* UrlData::clone() const
{
    UrlData* url = new UrlData();
    url->id = id;
    url->name = name;
    url->source = source;
    return url;
}
