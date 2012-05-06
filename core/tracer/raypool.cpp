#include "raypool.h"

RayPool::RayPool(QObject *parent) :
    QObject(parent),
    m_rays()
{
}

void RayPool::pushRay(Ray3D* ray)
{
    m_rays.append(ray);
}

Ray3D* RayPool::popRay()
{
    if (m_rays.isEmpty())
        return NULL;
    return m_rays.takeFirst();
}
