#ifndef LENSOBJECTSTUB_H
#define LENSOBJECTSTUB_H

#include "core/stubs/virtual3dobject_stub.h"
#include <QImage>

class LensObjectStub : public Virtual3DObjectStub
{
public:
    LensObjectStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2,
                   const QImage &heightMap1, const QImage &heightMap2,
                   double height, double refractiveIndex);

private:
    QImage m_heightMap1, m_heightMap2;
    double m_height, m_refractiveIndex;

};

#endif // LENSOBJECTSTUB_H
