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
    /* TODO: вот тут-то нам нужно побить поверхность на полигоны. */
}

void LensObject::triangulateSurfaces()
{
    m_frontPolygons = new PhysicalTetragonPolygon**[m_heightMap1.size().width()];
    for (int i = 0; i < m_heightMap1.size().width(); i++) {
        m_frontPolygons[i] = new PhysicalTetragonPolygon*[m_heightMap1.size().height()];
        for (int j = 0; j < m_heightMap1.size().height(); j++) {
            //TODO: координатки полигонов...
            //m_frontPolygons[i][j] = new PhysicalTetragonPolygon();
        }
    }

    m_backPolygons = new PhysicalTetragonPolygon**[m_heightMap2.size().width()];
    for (int i = 0; i < m_heightMap2.size().width(); i++) {
        m_backPolygons[i] = new PhysicalTetragonPolygon*[m_heightMap2.size().height()];
        for (int j = 0; j < m_heightMap2.size().height(); j++) {
            //TODO: координатки полигонов...
            //m_backPolygons[i][j] = new PhysicalTetragonPolygon();
        }
    }
}

Point3D* LensObject::intercrossWithRay(const Ray3D &ray)
{
    Point3D* p1 = 0;
    p1 = m_frontPolygon.intercrossWithRay(ray);
    p1 = (p1 == 0) ? m_frontLeftPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_frontRightPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_frontTopPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_frontBottomPolygon.intercrossWithRay(ray) : p1;

    Point3D* p2 = 0;
    p2 = m_backPolygon.intercrossWithRay(ray);
    p2 = (p2 == 0) ? m_backLeftPolygon.intercrossWithRay(ray) : p2;
    p2 = (p2 == 0) ? m_backRightPolygon.intercrossWithRay(ray) : p2;
    p2 = (p2 == 0) ? m_backTopPolygon.intercrossWithRay(ray) : p2;
    p2 = (p2 == 0) ? m_backBottomPolygon.intercrossWithRay(ray) : p2;

    bool back = false, front = false;

    if ((p1 != NULL) && (p2 != NULL)) {
        if (p1->dist(ray.point()) > p2->dist(ray.point()))
            back = true;
        else
            front = true;
        delete p1;
        delete p2;
    } else if (p1 != NULL) {
        delete p1;
        front = true;
    } else if (p2 != NULL) {
        delete p2;
        back = true;
    }

    // TODO: do something
    return NULL;
}

void LensObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
    Q_UNUSED(ray)
    Q_UNUSED(point)
}

void LensObject::setRefractiveIndex(double refractiveIndex)
{
    m_refractiveIndex = refractiveIndex;
}
