#include "physicalray.h"

PhysicalRay::PhysicalRay(const Point3D &p, const Vector3D &v, const int stx, const int sty, const double intensity)
    : Ray3D(p, v), m_stx(stx), m_sty(sty), m_intensity(intensity)
{
}

PhysicalRay::PhysicalRay(const Point3D &p1, const Point3D &p2, const int stx, const int sty, const double intensity)
    : Ray3D(p1, p2), m_stx(stx), m_sty(sty), m_intensity(intensity)
{
}
