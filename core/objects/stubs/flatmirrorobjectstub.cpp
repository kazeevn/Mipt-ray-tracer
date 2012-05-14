#include "flatmirrorobjectstub.h"

FlatMirrorObjectStub::FlatMirrorObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &bitmask, double reflcoef)
    :  Virtual3DObjectStub(point, v1,v2), m_bitmask(bitmask), m_reflcoef(reflcoef)
{
}

bool FlatMirrorObjectStub::isValid()
{
    if (!Virtual3DObjectStub::isValid())
        return false;
    if (m_bitmask.isNull())
        return false;
    if (m_reflcoef > 1 || m_reflcoef < 0)
        return false;
    return true;
}
