#include "ray3d.h"

Ray3D::Ray3D(const Point3D &p, const Vector3D &v)
    : p(p), v(v)
{
}

Ray3D::Ray3D(const Point3D &p1, const Point3D &p2)
    : p(p1), v(p1, p2)
{
}

Ray3D::Ray3D(double x1, double y1, double z1, double x2, double y2, double z2)
    : p(x1, y1, z1), v(x2-x1, y2-y1, z2-z1)
{
}
