#ifndef PLANE3D_H
#define PLANE3D_H

#include "core/point3d.h"
#include "core/ray3d.h"

class Plane3D
{
public:
    Plane3D(const Point3D& p1, const Point3D& p2, const Point3D& p3);
    Plane3D(double x1, double y1, double z1,
            double x2, double y2, double z2,
            double x3, double y3, double z3);
    Point3D *intercrossWithRay(const Ray3D& ray);
private:
    void calcPlaneCoef(const Point3D& p1, const Point3D& p2, const Point3D& p3);
    double A, B, C, D;
};

#endif // PLANE3D_H
