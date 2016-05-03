#ifndef ALIASDATA_H
#define ALIASDATA_H

#include <QtCore/QMetaType>
#include <QtCore/QString>

#include "data/basedata.h"

class AliasData: public BaseData
{
public:
    AliasData();
    virtual ~AliasData();

public:
    int getId() const;
    const QString& getAlias() const;

public:
    void setId(int value);
    void setAlias(const QString& value);

public:
    AliasData* clone() const;

private:
    int id;
    QString alias;

};

Q_DECLARE_METATYPE(AliasData*)

#endif // ALIASDATA_H
