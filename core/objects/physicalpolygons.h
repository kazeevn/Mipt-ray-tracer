#ifndef PHYSICALPOLYGONS_H
#define PHYSICALPOLYGONS_H

#include "core/objects/trianglepolygon.h"
#include "core/geometry/point3d.h"

#include <QDebug>

/* Класс отвечает за так называемый "физический" полигон
 * На самом деле это полигон границы раздела двух сред.
 * Среда 1 - та, в которую смотрит векторное произведение векторов
 * (p1-p2) и (p1-p3). Тогда показатель преломления задается
 * как отношение показателей преломления среды 2 и среды 1
 */

class PhysicalTrianglePolygon : public TrianglePolygon
{
public:
    PhysicalTrianglePolygon(const Point3D &p1, const Point3D &p2, const Point3D &p3);

    void processPhysicalIntersection(const Ray3D &ray, const Point3D &point, double refractiveIndex);
private:
    Vector3D m_perpendicular;
};

/* Класс, отвечающий за "физический" полигон с четырмя точками
 * Ничего особенного, просто обертка для "пары треугольных полигонов".
 * Аналогично треугольному полигону */
class PhysicalTetragonPolygon : public Virtual3DObject
{
public:
    PhysicalTetragonPolygon(const Point3D &p1, const Point3D &p2, const Point3D &p3, const Point3D &p4);

    Point3D* intercrossWithRay(const Ray3D &ray);
    void processPhysicalIntersection(const Ray3D &ray, const Point3D &point, double refractiveIndex);
    void processIntersection(const Ray3D &ray, const Point3D &point) {
        Q_UNUSED(ray)
        Q_UNUSED(point)
    }
private:
    PhysicalTrianglePolygon m_poly1, m_poly2;
};

// Отладочный шлак
QDebug operator <<(QDebug dbg, const PhysicalTetragonPolygon& p);

#endif // PHYSICALPOLYGONS_H
