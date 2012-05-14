#ifndef FLATMIRROROBJECT_H
#define FLATMIRROROBJECT_H

#include "core/objects/helpers/rectanglepolygon.h"
#include <QImage>

class FlatMirrorObject : public RectanglePolygon
{
public:
    FlatMirrorObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &bitmask, double reflcoef);
    Point3D* intercrossWithRay(const Ray3D &ray);
    void processIntersection(const PhysicalRay &ray, const Point3D &point);
private:
    QImage m_bitmask;
    Vector3D m_perpendicular;
    double m_reflcoef;
};

#endif // FLATMIRROROBJECT_H
