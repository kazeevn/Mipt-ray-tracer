#ifndef TRIANGLEPOLYGON_H
#define TRIANGLEPOLYGON_H

#include "core/objects/virtual3dobject.h"
#include "core/geometry/point3d.h"
#include "core/geometry/plane3d.h"

class TrianglePolygon : public Virtual3DObject
{
public:
    TrianglePolygon(const Point3D& p1, const Point3D& p2, const Point3D& p3);
    TrianglePolygon(double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double x3, double y3, double z3);
    Point3D *intercrossWithRay(const Ray3D &ray);
    // Just do nothing
    void processIntersection(const Ray3D &ray, const Point3D &point)
    {
        Q_UNUSED(ray);
        Q_UNUSED(point);
    };
    static bool isPointInTriangle2D(double x1, double y1,
                                    double x2, double y2,
                                    double x3, double y3,
                                    double cx, double cy);
private:
    // Points
    Point3D p1, p2, p3;
    // Plane
    Plane3D plane;
};

#endif // TRIANGLEPOLYGON_H