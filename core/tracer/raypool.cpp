#include "raypool.h"
#include <QMutexLocker>

RayPool::RayPool(QObject *parent)
    : QObject(parent),
      m_rays(),
      m_mutex()
{
}

void RayPool::pushRay(Ray3D* ray)
{
    QMutexLocker locker(&m_mutex);
    m_rays.append(ray);
    m_mutex.unlock();
}

Ray3D* RayPool::popRay()
{
    QMutexLocker locker(&m_mutex);
    if (m_rays.isEmpty())
        return NULL;
    return m_rays.takeFirst();
}
