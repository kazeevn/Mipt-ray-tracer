#include "virtual3dobjectstub.h"

#include <cfloat>

QList<Point3D> Virtual3DObjectStub :: getPoints() {
    QList<Point3D> points;

    points.append(point());

    points.append(point()+v1());
    points.append(point()+v1()+v2());
    points.append(point()+v2());

    return points;
}


bool Virtual3DObjectStub::isValid()
{
    if (v1().crossProduct(v2()).length() < DBL_EPSILON)
        return false;
    return true;
}
