#include "pictureobjectstub.h"

PictureObjectStub::PictureObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &image)
    :  Virtual3DObjectStub(point, v1,v2), m_image(image)
{
}



