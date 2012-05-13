#include "rectangle3d.h"

Rectangle3D::Rectangle3D(const Point3D &p, const Vector3D &v1, const Vector3D &v2)
    : m_point(p), m_v1(v1), m_v2(v2)
{
}

QDebug operator <<(QDebug dbg, const Rectangle3D& r)
{
    dbg.space() << "Rectangle(" << r.point() << "," << r.horizontalVect() << "," << r.verticalVect() << ")";
    return dbg.space();
}
