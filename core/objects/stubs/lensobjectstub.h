#ifndef LENSOBJECTSTUB_H
#define LENSOBJECTSTUB_H

#include "core/objects/stubs/virtual3dobjectstub.h"
#include <QImage>

class LensObjectStub : public Virtual3DObjectStub
{
public:
    LensObjectStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2,
                   const QImage &heightMap1, const QImage &heightMap2,
                   const QSize& size, double height, double refractiveIndex);
    double& height() {return m_height;}
    double& refractiveIndex () {return m_refractiveIndex;}
    QSize& size() { return m_size; }
    QImage& heightMap1() { return m_heightMap1; }
    QImage& heightMap2() { return m_heightMap2; }

    bool isValid();
private:
    QImage m_heightMap1, m_heightMap2;
    QSize m_size;
    double m_height, m_refractiveIndex;

};

#endif // LENSOBJECTSTUB_H
