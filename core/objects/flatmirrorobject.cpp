#include "flatmirrorobject.h"

#include "core/tracer/raypool.h"
#include <QColor>

FlatMirrorObject::FlatMirrorObject(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const QImage& bitmask, double reflcoef)
    : RectanglePolygon(point, v1, v2), m_bitmask(bitmask),
      m_perpendicular(v2.crossProduct(v1)),
      m_reflcoef(reflcoef)
{
}

Point3D* FlatMirrorObject::intercrossWithRay(const Ray3D &ray)
{
    /* Copypasted from ThinLensObjecet */
    Point3D* p = RectanglePolygon::intercrossWithRay(ray);
    if (p == NULL)
        return NULL;
    Vector3D oldvector(m_rectangle.point(), *p);
    Vector3D newvector = m_matrix.transformVector(oldvector);

    QColor c = QColor::fromRgb(m_bitmask.pixel(newvector.x*(m_bitmask.width()-1),
                                               newvector.y*(m_bitmask.height()-1)));
    // Черный цвет => непрозрачно
    if (c.blackF() > 0.5)
        return p;
    else {
        delete p;
        return NULL;
    }
}

void FlatMirrorObject::processIntersection(const PhysicalRay &ray, const Point3D &point)
{
    Vector3D projection = m_perpendicular*(ray.direction().dotProduct(m_perpendicular) / m_perpendicular.dotProduct(m_perpendicular));
    Vector3D newDirection = ray.direction() - projection*2;
    RayPool::Instance().pushRay(new PhysicalRay(point, newDirection, ray.startingX(), ray.startingY(), ray.intensity()*m_reflcoef));
}


