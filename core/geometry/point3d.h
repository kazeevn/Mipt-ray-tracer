#ifndef POINT3D_H
#define POINT3D_H

#include <QDebug>

class Vector3D;

class Point3D
{
public:
    Point3D(double x = 0, double y = 0, double z = 0);
    double dist(const Point3D& point);

    Point3D operator+(const Vector3D& v) const;
    Point3D operator-(const Vector3D& v) const;
    double x, y, z;
    double& operator[](const int i);
};

QDebug operator<<(QDebug dbg, const Point3D& p);

#endif // POINT3D_H
