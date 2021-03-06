#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "core/geometry/point3d.h"
#include <cmath>
#include <QDebug>

class Vector3D
{
public:
    Vector3D(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
    Vector3D(double x1, double y1, double z1, double x2, double y2, double z2);
    Vector3D(const Point3D& p1, const Point3D& p2);

    double length() const { return sqrt(x*x+y*y+z*z); }

    double dotProduct(const Vector3D& v) const;
    Vector3D crossProduct(const Vector3D& v) const;

    Vector3D unit() const;

    Vector3D operator- (const Vector3D& v) const;
    Vector3D operator+ (const Vector3D& v) const;
    Vector3D operator* (const double mult) const;
    Vector3D operator/ (const double mult) const;
    Vector3D& operator*=(const double mult);
    Vector3D& operator/=(const double mult);
    Vector3D& operator+=(const Vector3D& v);
    double x, y, z;
    double& operator[](const int i);
};

QDebug operator<<(QDebug dbg, const Vector3D& v);

#endif // VECTOR3D_H
