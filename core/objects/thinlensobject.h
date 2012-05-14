#ifndef THINLENSOBJECT_H
#define THINLENSOBJECT_H

#include "core/objects/helpers/rectanglepolygon.h"
#include "core/geometry/plane3d.h"
#include "core/geometry/vector3d.h"
#include <QImage>

class ThinLensObject : public RectanglePolygon
{
public:
    ThinLensObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage& bitmask, double focus);
    Point3D* intercrossWithRay(const Ray3D &ray);
    void processIntersection(const PhysicalRay &ray, const Point3D &point);

private:
    Point3D opticalCentre;
    QImage m_bitmask;
    double m_focus;

    Vector3D m_perpendicular;

    Plane3D m_frontFocalPlane;
    Plane3D m_backFocalPlane;
};

#endif // THINLENSOBJECT_H
