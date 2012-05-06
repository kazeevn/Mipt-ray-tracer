#include "point3d.h"

double Point3D::dist(const Point3D &point)
{
    return sqrt((point.x - x)*(point.x - x) +
                (point.y - y)*(point.y - y) +
                (point.z - z)*(point.z - z));
}
