#include "lensobject.h"

LensObject::LensObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &heightMap1, const QImage &heightMap2, double height, double refractiveIndex)
    : m_rectangle(point, v1, v2), m_heightMap1(heightMap1), m_heightMap2(heightMap2),
      m_height(height), m_refractiveIndex(refractiveIndex),
      m_perpendicular(v2.crossProduct(v1).unit()*height),

      m_frontPolygon(point+m_perpendicular, v1, v2),
      m_backPolygon(point-m_perpendicular, v2, v1),
      m_leftPolygon(point-m_perpendicular, v2, m_perpendicular*2),
      m_rightPolygon(point+v1-m_perpendicular, m_perpendicular*2, v2),
      m_topPolygon(point-m_perpendicular, m_perpendicular*2, v1),
      m_bottomPolygon(point+v2-m_perpendicular, v1, m_perpendicular*2)

{
}

Point3D* LensObject::intercrossWithRay(const Ray3D &ray)
{
}

void LensObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
}
