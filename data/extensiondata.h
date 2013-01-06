#ifndef EXTENSIONDATA_H
#define EXTENSIONDATA_H

#include <QtCore/QString>

class ExtensionData
{
public:
    enum ExtensionTypeEnum {
        ExtensionImage = 1,
        ExtensionVideo
    };

public:
    ExtensionData(int id, const QString& name, ExtensionTypeEnum type);
    virtual ~ExtensionData();

public:
    ExtensionTypeEnum getType() const;
    const QString& getName() const;
    int getId() const;

private:
    ExtensionTypeEnum type;
    QString name;
    int id;
};

#endif // EXTENSIONDATA_H
