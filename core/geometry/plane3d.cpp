#include "plane3d.h"
#include <cmath>
#include <cfloat>

Plane3D::Plane3D(const Point3D &p1, const Point3D &p2, const Point3D &p3)
{
    calcPlaneCoef(p1, p2, p3);
}

Plane3D::Plane3D(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
{
    Point3D p1(x1, y1, z1);
    Point3D p2(x2, y2, z2);
    Point3D p3(x3, y3, z3);
    calcPlaneCoef(p1, p2, p3);
}

void Plane3D::calcPlaneCoef(const Point3D &p1, const Point3D &p2, const Point3D &p3)
{
    A = p3.z*p1.y + p3.y*p2.z + p2.y*p1.z - p2.z*p1.y - p2.y*p3.z - p3.y*p1.z;
    B = p2.x*p3.z + p1.x*p2.z + p3.x*p1.z - p1.x*p3.z - p2.z*p3.x - p2.x*p1.z;
    C = p1.x*p3.y + p3.x*p2.y + p2.x*p1.y - p1.x*p2.y - p2.x*p3.y - p3.x*p1.y;
    D = p1.x*p2.y*p3.z + p2.x*p3.y*p1.z + p3.x*p1.y*p2.z - p1.x*p3.y*p3.z - p2.x*p1.y*p3.z - p3.x*p2.y*p1.z;
}

Point3D *Plane3D::intercrossWithRay(const Ray3D &ray)
{
    const Point3D& p = ray.getp();
    const Vector3D& v = ray.getv();
    double denom = A*v.x + B*v.y + C*v.z;
    if (fabs(denom) < DBL_EPSILON)
        return 0;
    double t = -(A*p.x + B*p.y + C*p.z + D)/denom;
    // Do not intercross with plane if the starting point is already on the plane
    if (t < DBL_EPSILON)
        return 0;
    return new Point3D(p.x + t*v.x, p.y + t*v.y, p.z + t*v.z);
}
