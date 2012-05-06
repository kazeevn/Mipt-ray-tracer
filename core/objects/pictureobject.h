#ifndef PICTUREOBJECT_H
#define PICTUREOBJECT_H

#include "core/objects/virtual3dobject.h"

class PictureObject : public Virtual3DObject
{
public:
    PictureObject(QPixmap &pixmap);
private:
    QPixmap& m_pixmap;

};

#endif // PICTUREOBJECT_H
