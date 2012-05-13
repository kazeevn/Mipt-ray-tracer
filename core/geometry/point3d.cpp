#include "point3d.h"

#include "core/geometry/vector3d.h"
 #include <QtGlobal>
#include <cmath>

Point3D::Point3D(double x, double y, double z)
    : x(x), y(y), z(z)
{
}

double Point3D::dist(const Point3D &point)
{
    return sqrt((point.x - x)*(point.x - x) +
                (point.y - y)*(point.y - y) +
                (point.z - z)*(point.z - z));
}

Point3D Point3D::operator +(const Vector3D& v) const
{
    Point3D p(x+v.x, y+v.y, z+v.z);
    return p;
}

Point3D Point3D::operator -(const Vector3D& v) const
{
    Point3D p(x-v.x, y-v.y, z-v.z);
    return p;
}

double& Point3D::operator [](int i) {
    Q_ASSERT(i<3);
    switch (i) {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    case 2:
        return z;
        break;
    }
}
