#ifndef VIRTUAL3DOBJECT_H
#define VIRTUAL3DOBJECT_H

#include "core/ray3d.h"

class Virtual3DObject
{
public:
    Virtual3DObject() { }
    virtual Point3D* intercrossWithRay(Ray3D &ray) {
        return 0;
    }
};

#endif // VIRTUAL3DOBJECT_H
