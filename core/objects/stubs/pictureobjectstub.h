#ifndef PICTUREOBJECT_STUB_H
#define PICTUREOBJECT_STUB_H

#include <QImage>

#include"core/objects/stubs/virtual3dobjectstub.h"

class PictureObjectStub : public Virtual3DObjectStub {
public:
    PictureObjectStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const QImage &image);
    void setImage(const QImage &image) { m_image = image; }
    QImage& image() { return m_image; }
private:
    QImage m_image;
};

#endif // PICTUREOBJECT_STUB_H
