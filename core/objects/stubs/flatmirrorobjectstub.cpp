#include "flatmirrorobjectstub.h"

FlatMirrorObjectStub::FlatMirrorObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &bitmask, double reflcoef)
    :  Virtual3DObjectStub(point, v1,v2), m_bitmask(bitmask), m_reflcoef(reflcoef)
{
}
