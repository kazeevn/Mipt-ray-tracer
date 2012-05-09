#include "lensobject.h"

LensObject::LensObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &heightMap1, const QImage &heightMap2, double height, double refractiveIndex)
    : m_rectangle(point, v1, v2), m_heightMap1(heightMap1), m_heightMap2(heightMap2),
      m_height(height), m_refractiveIndex(refractiveIndex),
      m_perpendicular(v2.crossProduct(v1).unit()*height),

      m_frontPolygon(point+m_perpendicular, v1, v2),
      m_frontLeftPolygon(point, v2, m_perpendicular),
      m_frontRightPolygon(point+v1, v2, m_perpendicular),
      m_frontTopPolygon(point, m_perpendicular, v1),
      m_frontBottomPolygon(point+v2, v1, m_perpendicular),

      m_backPolygon(point-m_perpendicular, v2, v1),
      m_backLeftPolygon(point, m_perpendicular*(-1), v2),
      m_backRightPolygon(point+v1, v2, m_perpendicular*(-1)),
      m_backTopPolygon(point, v1, m_perpendicular*(-1)),
      m_backBottomPolygon(point+v2, m_perpendicular*(-1), v1)
{
}

Point3D* LensObject::intercrossWithRay(const Ray3D &ray)
{
    Point3D* p1 = 0;
    p1 = m_frontPolygon.intercrossWithRay(ray);
    p1 = p1 == 0 ? m_frontLeftPolygon.intercrossWithRay(ray) : p1;
    p1 = p1 == 0 ? m_frontRightPolygon.intercrossWithRay(ray) : p1;
    p1 = p1 == 0 ? m_frontTopPolygon.intercrossWithRay(ray) : p1;
    p1 = p1 == 0 ? m_frontBottomPolygon.intercrossWithRay(ray) : p1;
    if (!p1) {
        delete p1;
        // TODO: Скорее всего, мы пересеклись с одним из "передних" полигонов
        // НАЙТИ И УНИЧТОЖИТЬ!
        return NULL;
    }
    p1 = m_backPolygon.intercrossWithRay(ray);
    p1 = p1 == 0 ? m_backLeftPolygon.intercrossWithRay(ray) : p1;
    p1 = p1 == 0 ? m_backRightPolygon.intercrossWithRay(ray) : p1;
    p1 = p1 == 0 ? m_backTopPolygon.intercrossWithRay(ray) : p1;
    p1 = p1 == 0 ? m_backBottomPolygon.intercrossWithRay(ray) : p1;
    if (!p1) {
        delete p1;
        // TODO: аналогично, только с "задними"
        return NULL;
    }
    return NULL;
}

void LensObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
    Q_UNUSED(ray)
    Q_UNUSED(point)
}
