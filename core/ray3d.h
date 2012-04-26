#ifndef RAY3D_H
#define RAY3D_H

#include "core/point3d.h"
#include "core/vector3d.h"

/* Class represents a geometric ray starting from point p
 * and running in the direction of vector v
 */
class Ray3D
{
public:
    Ray3D(const Point3D& p, const Vector3D& v);
    Ray3D(const Point3D& p1, const Point3D& p2);
    Ray3D(double x, double y, double z, double x1, double y1, double z2);
    const Point3D& getp() const { return p; }
    const Vector3D& getv() const { return v; }
private:
    Point3D p;
    Vector3D v;
};

#endif // RAY3D_H
