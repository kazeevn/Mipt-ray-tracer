#ifndef PHYSICALTRIANGLEPOLYGON_H
#define PHYSICALTRIANGLEPOLYGON_H

#include "core/objects/helpers/trianglepolygon.h"
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

    void processPhysicalIntersection(const PhysicalRay &ray, const Point3D &point, double refractiveIndex);
private:
    Vector3D m_perpendicular;
};

#endif // PHYSICALTRIANGLEPOLYGON_H
