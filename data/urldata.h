#ifndef URLDATA_H
#define URLDATA_H

#include <QtCore/QMetaType>
#include <QtCore/QString>

#include "data/basedata.h"

class UrlData: public BaseData
{
public:
    UrlData();
    virtual ~UrlData();

public:
    int getId() const;
    const QString& getName() const;
    const QString& getSource() const;

public:
    void setId(int value);
    void setName(const QString& value);
    void setSource(const QString& value);

public:
    UrlData* clone() const;

private:
    int id;
    QString name;
    QString source;

};

Q_DECLARE_METATYPE(UrlData*)

#endif // URLDATA_H
