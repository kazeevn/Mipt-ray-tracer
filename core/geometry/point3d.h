#ifndef POINT3D_H
#define POINT3D_H

class Vector3D;

class Point3D
{
public:
    Point3D(double x = 0, double y = 0, double z = 0);
    double dist(const Point3D& point);

    Point3D operator+(const Vector3D& v) const;
    Point3D operator-(const Vector3D& v) const;
    double x, y, z;
};

#endif // POINT3D_H
