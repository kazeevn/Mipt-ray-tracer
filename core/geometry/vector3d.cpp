#include "vector3d.h"

Vector3D::Vector3D(double x1, double y1, double z1, double x2, double y2, double z2)
    : x(x2-x1), y(y2-y1), z(z2-z1)
{
}

Vector3D::Vector3D(const Point3D &p1, const Point3D &p2)
    : x(p2.x-p1.x), y(p2.y-p1.y), z(p2.z-p1.z)
{
}
