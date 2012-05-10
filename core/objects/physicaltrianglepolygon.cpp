#include "physicaltrianglepolygon.h"
#include "core/tracer/physicalray.h"

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
}
