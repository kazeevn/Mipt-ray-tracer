#ifndef VIRTUAL3DOBJECT_H
#define VIRTUAL3DOBJECT_H

#include "core/geometry/ray3d.h"
#include <QString>

class Virtual3DObject
{
public:
    Virtual3DObject() {}
    virtual Point3D* intercrossWithRay(const Ray3D &ray) = 0;
    virtual void processIntersection(const Ray3D &ray, const Point3D& point) = 0;
    const QString& name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
private:
    QString m_name;
};

#endif // VIRTUAL3DOBJECT_H
