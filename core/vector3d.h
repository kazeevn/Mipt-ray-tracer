#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "core/point3d.h"

class Vector3D
{
public:
    Vector3D() { x = y = z = 0; }
    Vector3D(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
    Vector3D(double x1, double y1, double z1, double x2, double y2, double z2);
    Vector3D(const Point3D& p1, const Point3D& p2);

    double getx() const { return x; }
    double gety() const { return y; }
    double getz() const { return z; }
private:
    double x, y, z;
};

#endif // VECTOR3D_H
