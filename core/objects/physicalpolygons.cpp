#include "physicalpolygons.h"
#include "core/tracer/physicalray.h"
#include "core/tracer/raypool.h"

#include <cmath>
#include <cfloat>

PhysicalTrianglePolygon::PhysicalTrianglePolygon(const Point3D &p1, const Point3D &p2, const Point3D &p3)
    : TrianglePolygon(p1, p2, p3),
      m_perpendicular(Vector3D(p1, p2).crossProduct(Vector3D(p1, p3)))
{
}

void PhysicalTrianglePolygon::processPhysicalIntersection(const Ray3D &ray, const Point3D &point, double refractiveIndex)
{
    const PhysicalRay* physray = dynamic_cast<const PhysicalRay*>(&ray);
    if (!physray)
        return;
    double dotProduct = physray->direction().dotProduct(m_perpendicular);

    Vector3D localPerpendicular(dotProduct > 0 ? m_perpendicular*(-1) : m_perpendicular);
    double localRefractiveIndex = (dotProduct < 0 ? refractiveIndex : 1 / refractiveIndex);
    // Проекция падающего луча на нормаль
    Vector3D projection(localPerpendicular*(dotProduct/localPerpendicular.dotProduct(localPerpendicular)));
    projection *= dotProduct / m_perpendicular.dotProduct(m_perpendicular);
    // Направление отраженного луча
    Vector3D reflectedDirection(physray->direction() - projection*2);
    double incidenceAngle = acos(dotProduct / m_perpendicular.length() / physray->direction().length());
    // Полное внутреннее отражение
    if (sin(incidenceAngle)/localRefractiveIndex >= 1) {
        RayPool::Instance().pushRay(new PhysicalRay(point, reflectedDirection,
                                                    physray->startingX(), physray->startingY(), physray->intensity()));
        return;
    }
    // Иначе: угол преломления
    double fractureAngle = asin(sin(incidenceAngle) / localRefractiveIndex);
    // Проекция падающего луча на плоскость треугольника
    Vector3D planeProjection(physray->direction() - projection);
    // Направление преломленного луча:
    Vector3D fractureDirection(projection.unit()*cos(fractureAngle));
    if (fractureAngle > DBL_EPSILON)
        fractureDirection += planeProjection.unit()*sin(fractureAngle);
    // Коэффициенты отражения и преломления для разных поляризаций
    //double sfrac = 2*cos(incidenceAngle)*sin(fractureAngle) / sin(fractureAngle+incidenceAngle);
    double srefl = -sin(incidenceAngle - fractureAngle) / sin(incidenceAngle + fractureAngle);
    //double pfrac = 2*cos(incidenceAngle)*sin(fractureAngle) / sin(incidenceAngle+fractureAngle) / cos(incidenceAngle - fractureAngle);
    double prefl = tan(incidenceAngle-fractureAngle) / tan(incidenceAngle+fractureAngle);
    // Отраженный луч
    RayPool::Instance().pushRay(new PhysicalRay(point, reflectedDirection,
                                                physray->startingX(), physray->startingY(),
                                                (srefl*srefl + prefl*prefl)/2 * physray->intensity()));
    // Преломлённый луч
    RayPool::Instance().pushRay(new PhysicalRay(point, fractureDirection,
                                                physray->startingX(), physray->startingY(),
                                                (2-srefl*srefl-prefl*prefl)/2 * physray->intensity()));
}


PhysicalTetragonPolygon::PhysicalTetragonPolygon(const Point3D &p1, const Point3D &p2, const Point3D &p3, const Point3D &p4)
    : m_poly1(p1, p2, p4), m_poly2(p2, p3, p4)
{
}

Point3D* PhysicalTetragonPolygon::intercrossWithRay(const Ray3D &ray)
{
    Point3D* p = m_poly1.intercrossWithRay(ray);
    if (p) return p;
    p = m_poly2.intercrossWithRay(ray);
    if (p) return p;
    return NULL;
}

void PhysicalTetragonPolygon::processPhysicalIntersection(const Ray3D &ray, const Point3D &point, double refractiveIndex)
{
    Point3D* p = m_poly1.intercrossWithRay(ray);
    if (p && (p->dist(point) < DBL_EPSILON)) {
        delete p;
        m_poly1.processPhysicalIntersection(ray, point, refractiveIndex);
    } else
        m_poly2.processPhysicalIntersection(ray, point, refractiveIndex);
}
