#ifndef POINT3D_H
#define POINT3D_H

class Point3D
{
public:
    Point3D(double x = 0, double y = 0, double z = 0) { this->x = x; this->y = y; this->z = z; }
    double x, y, z;
};

#endif // POINT3D_H
