#ifndef RAY3D_H
#define RAY3D_H

#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"

/* Class represents a geometric ray starting from point p
 * and running in the direction of vector v
 */
class Ray3D
{
public:
    Ray3D(const Point3D& p, const Vector3D& v);
    Ray3D(const Point3D& p1, const Point3D& p2);
    Ray3D(double x, double y, double z, double x1, double y1, double z2);
    virtual ~Ray3D() {}
    const Point3D& point() const { return m_p; }
    const Vector3D& direction() const { return m_v; }
private:
    Point3D m_p;
    Vector3D m_v;
};

#endif // RAY3D_H
