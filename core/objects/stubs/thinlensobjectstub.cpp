#include "thinlensobjectstub.h"

#include <cfloat>

ThinLensObjectStub::ThinLensObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &bitmask, double focus)
    :  Virtual3DObjectStub(point, v1,v2), m_bitmask(bitmask), m_focus(focus)
{
}

bool ThinLensObjectStub::isValid()
{
    if (!Virtual3DObjectStub::isValid())
        return false;
    if (m_bitmask.isNull())
        return false;
    if (fabs(m_focus) < DBL_EPSILON)
        return false;
    return true;
}
