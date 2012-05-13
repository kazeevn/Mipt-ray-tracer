#include "virtual3dobject_stub.h"

QList<Point3D> Virtual3DObjectStub :: getPoints() {
    QList<Point3D> points;

    points.append(point());

    points.append(point()+v1());
    points.append(point()+v1()+v2());

    points.append(point()+v2());



    return points;
}
