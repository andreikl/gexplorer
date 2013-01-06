#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QtCore/QString>

class CustomGalleryItemData;
class GalleryItemData;

class CommonHelper {
public:
    static QString getPath(const GalleryItemData& item);
    static int getAngle(const GalleryItemData& item);

    static void getChildren(const CustomGalleryItemData& item, QList<CustomGalleryItemData*>& children);
    static QString getPath(const CustomGalleryItemData& item);
    static int getAngle(const CustomGalleryItemData& item);
};

#endif // COMMONHELPER_H
