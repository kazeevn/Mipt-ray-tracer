#include "rectanglepolygon.h"

RectanglePolygon::RectanglePolygon(const Point3D &point, const Vector3D &v1, const Vector3D &v2)
    : m_rectangle(point, v1, v2),
      m_poly1(point, point+v1, point+v2),
      m_poly2(point+v1, point+v2, point+v1+v2)
{
}

Point3D* RectanglePolygon::intercrossWithRay(const Ray3D &ray)
{
    Point3D* p = m_poly1.intercrossWithRay(ray);
    if (p) return p;
    p = m_poly2.intercrossWithRay(ray);
    return p;
}
