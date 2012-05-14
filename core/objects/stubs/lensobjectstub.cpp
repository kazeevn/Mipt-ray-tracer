#include "lensobjectstub.h"

LensObjectStub::LensObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &heightMap1, const QImage &heightMap2, const QSize &size, double height, double refractiveIndex)
    : Virtual3DObjectStub(point, v1, v2),
      m_heightMap1(heightMap1), m_heightMap2(heightMap2),
      m_size(size), m_height(height),m_refractiveIndex(refractiveIndex)
{
}

bool LensObjectStub::isValid()
{
    if (!Virtual3DObjectStub::isValid())
        return false;
    if (m_heightMap1.isNull() || m_heightMap2.isNull())
        return false;
    if (m_size.isEmpty() || m_size.isNull())
        return false;
    if (m_height < 1e-2)
        return false;
    if (m_refractiveIndex < 1e-2)
        return false;
    return true;
}
