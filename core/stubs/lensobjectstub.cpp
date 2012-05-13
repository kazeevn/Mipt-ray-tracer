#include "lensobjectstub.h"

LensObjectStub::LensObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &heightMap1, const QImage &heightMap2, double height, double refractiveIndex)
    : Virtual3DObjectStub(point, v1, v2),
      m_heightMap1(heightMap1), m_heightMap2(heightMap2)
{
}
