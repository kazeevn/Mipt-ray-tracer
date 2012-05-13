#ifndef PICTUREOBJECT_STUB_H
#define PICTUREOBJECT_STUB_H

#include <QImage>

#include"core/objects/virtual3dobject_stub.h"

class PictureObjectStub : public Virtual3DObjectStub {
public:
    PictureObjectStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const QImage &image) :  Virtual3DObjectStub(point, v1,v2), m_image(image) {};
    void setImage(const QImage &image) {m_image=image;}
private:
    QImage m_image;
};

#endif // PICTUREOBJECT_STUB_H
