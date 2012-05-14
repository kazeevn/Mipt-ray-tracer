#ifndef TRIANGLEPOLYGON_H
#define TRIANGLEPOLYGON_H

#include "core/objects/virtual3dobject.h"
#include "core/geometry/point3d.h"
#include "core/geometry/plane3d.h"

#include <QDebug>

class TrianglePolygon : public Virtual3DObject
{
public:
    TrianglePolygon(const Point3D& p1, const Point3D& p2, const Point3D& p3);
    TrianglePolygon(double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double x3, double y3, double z3);
    const Point3D& point1() const { return p1; }
    const Point3D& point2() const { return p2; }
    const Point3D& point3() const { return p3; }
    Point3D *intercrossWithRay(const Ray3D &ray);
    // Just do nothing
    virtual void processIntersection(const PhysicalRay &ray, const Point3D &point)
    {
        Q_UNUSED(ray);
        Q_UNUSED(point);
    }
private:
    // Points
    Point3D p1, p2, p3;
    // Plane
    Plane3D plane;
    // Helper functions
    static bool isPointInTriangle2D(double x1, double y1,
                                    double x2, double y2,
                                    double x3, double y3,
                                    double cx, double cy);
    static bool isPointsInSamePlanePart(double x1, double y1,
                                        double x2, double y2,
                                        double cx1, double cy1,
                                        double cx2, double cy2);
};

QDebug operator<<(QDebug dbg, const TrianglePolygon& p);

#endif // TRIANGLEPOLYGON_H
