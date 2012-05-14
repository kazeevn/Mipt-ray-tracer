#include "physicaltrianglepolygon.h"
#include "core/tracer/physicalray.h"
#include "core/tracer/raypool.h"

#include <cmath>
#include <cfloat>

PhysicalTrianglePolygon::PhysicalTrianglePolygon(const Point3D &p1, const Point3D &p2, const Point3D &p3)
    : TrianglePolygon(p1, p2, p3),
      m_perpendicular(Vector3D(p1, p2).crossProduct(Vector3D(p1, p3)))
{
}

void PhysicalTrianglePolygon::processPhysicalIntersection(const PhysicalRay &ray, const Point3D &point, double refractiveIndex)
{
    double dotProduct = ray.direction().dotProduct(m_perpendicular);
    Vector3D localPerpendicular(dotProduct > 0 ? m_perpendicular*(-1) : m_perpendicular);
    double localRefractiveIndex = (dotProduct < 0 ? refractiveIndex : 1 / refractiveIndex);
    // Проекция падающего луча на нормаль
    Vector3D projection(localPerpendicular*(dotProduct/localPerpendicular.dotProduct(localPerpendicular)));
    // Направление отраженного луча
    Vector3D reflectedDirection(ray.direction() - projection*2);
    double incidenceAngle = acos( fabs(dotProduct) / m_perpendicular.length() / ray.direction().length());
    // Полное внутреннее отражение
    if (sin(incidenceAngle)/localRefractiveIndex >= 1) {
        RayPool::Instance().pushRay(new PhysicalRay(point, reflectedDirection,
                                                    ray.startingX(), ray.startingY(), ray.intensity()));
        return;
    }
    // Иначе: угол преломления
    double fractureAngle = asin(sin(incidenceAngle) / localRefractiveIndex);
    // Проекция падающего луча на плоскость треугольника
    Vector3D planeProjection(ray.direction() - projection);
    // Направление преломленного луча:
    Vector3D fractureDirection(projection.unit()*cos(fractureAngle));
    if (fractureAngle > DBL_EPSILON) {
        fractureDirection += planeProjection.unit()*sin(fractureAngle);
    }
    // Коэффициенты отражения и преломления для разных поляризаций
    //double sfrac = 2*cos(incidenceAngle)*sin(fractureAngle) / sin(fractureAngle+incidenceAngle);
    double srefl = -sin(incidenceAngle - fractureAngle) / sin(incidenceAngle + fractureAngle);
    //double pfrac = 2*cos(incidenceAngle)*sin(fractureAngle) / sin(incidenceAngle+fractureAngle) / cos(incidenceAngle - fractureAngle);
    double prefl = tan(incidenceAngle-fractureAngle) / tan(incidenceAngle+fractureAngle);


    /*qDebug() << "POLYGON" << *this;
    qDebug() << "RAY" << ray.direction();
    qDebug() << "PROCESS" << point << "incidence angle:" << incidenceAngle*180/3.1415926535 << "fracture angle:" << fractureAngle*180/3.1415926535;
    qDebug() << "P-polarization:" << prefl;
    qDebug() << "S-polarization:" << srefl;
    qDebug() << "REFLECTED INTENSITY" << (srefl*srefl + prefl*prefl)/2;
    qDebug() << "THRU INTENSITY" << (2-srefl*srefl-prefl*prefl)/2;
    qDebug() << " --- ";*/

    // Отраженный луч
    RayPool::Instance().pushRay(new PhysicalRay(point, reflectedDirection,
                                                ray.startingX(), ray.startingY(),
                                                (srefl*srefl + prefl*prefl)/2 * ray.intensity()));

    // Преломлённый луч
    RayPool::Instance().pushRay(new PhysicalRay(point, fractureDirection,
                                                ray.startingX(), ray.startingY(),
                                                (2-srefl*srefl-prefl*prefl)/2 * ray.intensity()));
}
