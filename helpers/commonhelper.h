#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QtCore/QString>

class CustomGalleryItemData;
class GalleryItemData;

class CommonHelper {
public:
    static QString getPath(const GalleryItemData& item);
    static QString getPath(const CustomGalleryItemData& item);

};

#endif // COMMONHELPER_H
