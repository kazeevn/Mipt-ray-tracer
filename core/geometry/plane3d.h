#ifndef PLANE3D_H
#define PLANE3D_H

#include "core/geometry/point3d.h"
#include "core/geometry/ray3d.h"
#include "core/geometry/vector3d.h"

class Plane3D
{
public:
    /* Задание плоскости по точке и вектору нормали */
    Plane3D(const Point3D& p, const Vector3D& v);
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
