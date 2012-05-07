#include "trianglepolygon.h"

TrianglePolygon::TrianglePolygon(const Point3D &p1, const Point3D &p2, const Point3D &p3)
    : p1(p1), p2(p2), p3(p3), plane(p1, p2, p3)
{
}

TrianglePolygon::TrianglePolygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3)
    : p1(x1, y1, z1), p2(x2, y2, z2), p3(x3, y3, z3), plane(p1, p2, p3)
{
}

bool TrianglePolygon::isPointsInSamePlanePart(double x1, double y1, double x2, double y2, double cx1, double cy1, double cx2, double cy2)
{
    double A = y2-y1;
    double B = x1-x2;
    double C = x2*y1 - x1*y2;
    double v1 = A*cx1 + B*cy1 + C;
    double v2 = A*cx2 + B*cy2 + C;
    return (((v1 >= 0) && (v2 >= 0)) || ((v1 <= 0)&&(v2 <= 0)));
}

bool TrianglePolygon::isPointInTriangle2D(double x1, double y1, double x2, double y2, double x3, double y3, double cx, double cy)
{
    return TrianglePolygon::isPointsInSamePlanePart(x1, y1, x2, y2, x3, y3, cx, cy) \
            && TrianglePolygon::isPointsInSamePlanePart(x1, y1, x3, y3, x2, y2, cx, cy) \
            && TrianglePolygon::isPointsInSamePlanePart(x2, y2, x3, y3, x1, y1, cx, cy);
}

Point3D* TrianglePolygon::intercrossWithRay(const Ray3D &ray)
{
    Point3D* intP = plane.intercrossWithRay(ray);
    if (!intP)
        return NULL;
    if (isPointInTriangle2D(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, intP->x, intP->y)
            && isPointInTriangle2D(p1.x, p1.z, p2.x, p2.z, p3.x, p3.z, intP->x, intP->z)
            && isPointInTriangle2D(p1.y, p1.z, p2.y, p2.z, p3.y, p3.z, intP->y, intP->z))
    {
        return intP;
    }
    else {
        delete intP;
        return NULL;
    }
}
