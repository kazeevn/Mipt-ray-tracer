#include "virtual3dobject_stub.h"

QList<Point3D> Virtual3DObjectStub :: getPoints() {
    QList<Point3D> points;

    points.append(point());

    Point3D newPoint1;
    newPoint1.x = point().x + v1().x;
    newPoint1.y = point().y + v1().y;
    newPoint1.z = point().z + v1().z;
    points.append(newPoint1);

    Point3D newPoint2;
    newPoint2.x = point().x + v2().x + v1().x;
    newPoint2.y = point().y + v2().y + v1().x;
    newPoint2.z = point().z + v2().z + v1().x;
    points.append(newPoint2);

    Point3D newPoint3;
    newPoint3.x = point().x + v2().x;
    newPoint3.y = point().y + v2().y;
    newPoint3.z = point().z + v2().z;
    points.append(newPoint3);

    return points;
}
