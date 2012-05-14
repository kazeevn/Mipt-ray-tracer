#include "thinlensobject.h"

#include <cfloat>
#include "core/tracer/physicalray.h"
#include "core/tracer/raypool.h"

#include <QColor>

ThinLensObject::ThinLensObject(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const QImage& bitmask, double focus)
    : RectanglePolygon(point, v1, v2),
      opticalCentre(point+v1/2+v2/2),
      m_bitmask(bitmask), m_focus(focus),
      m_perpendicular(v2.crossProduct(v1).unit()),
      m_frontFocalPlane(opticalCentre+m_perpendicular*m_focus, m_perpendicular),
      m_backFocalPlane(opticalCentre+m_perpendicular*(-m_focus), m_perpendicular)
{
}

Point3D* ThinLensObject::intercrossWithRay(const Ray3D &ray)
{
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

void ThinLensObject::processIntersection(const PhysicalRay &ray, const Point3D &point)
{
    if (ray.startingX() == 154 && ray.startingY() == 363) {
        qDebug() << "OH HAI";
        qDebug() << ray.point() << point;
    }
    Point3D *p = 0;
    Ray3D helperRay(opticalCentre, ray.direction());
    if (((p = m_frontFocalPlane.intercrossWithRay(helperRay)) != NULL) ||
            ((p = m_backFocalPlane.intercrossWithRay(helperRay)) != NULL)) {
        RayPool::Instance().pushRay(new PhysicalRay(point, Point3D(*p), ray.startingX(), ray.startingY(), ray.intensity()));
        delete p;
    }
}
