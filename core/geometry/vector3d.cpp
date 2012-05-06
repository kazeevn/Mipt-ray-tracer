#include "vector3d.h"

Vector3D::Vector3D(double x1, double y1, double z1, double x2, double y2, double z2)
    : x(x2-x1), y(y2-y1), z(z2-z1)
{
}

Vector3D::Vector3D(const Point3D &p1, const Point3D &p2)
    : x(p2.x-p1.x), y(p2.y-p1.y), z(p2.z-p1.z)
{
}

/* Возвращает векторное произведение НАШЕГО вектора на вектор v */
Vector3D Vector3D::crossProduct(const Vector3D &v)
{
    Vector3D res(y*v.z - z*v.y,
                 z*v.x - x*v.z,
                 x*v.y - y*v.x);
    return res;
}
