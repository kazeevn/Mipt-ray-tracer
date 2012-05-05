#ifndef VIRTUAL3DOBJECT_H
#define VIRTUAL3DOBJECT_H

#include "core/ray3d.h"
#include <QtGlobal>

class Virtual3DObject
{
public:
    Virtual3DObject() { }
    virtual Point3D* intercrossWithRay(const Ray3D &ray) {
        Q_UNUSED(ray);
        return 0;
    }
    virtual void processIntersection(const Ray3D &ray, const Point3D& point) {
        Q_UNUSED(ray);
        Q_UNUSED(point);
    }
};

#endif // VIRTUAL3DOBJECT_H
