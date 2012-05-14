#include "raypool.h"
#include <QMutexLocker>

RayPool::RayPool(QObject *parent)
    : QObject(parent),
      m_rays(),
      m_mutex()
{
}

void RayPool::pushRay(PhysicalRay *ray)
{
    QMutexLocker locker(&m_mutex);
    if (ray->intensity() < 4e-3)
        return;
    m_rays.push(ray);
}

PhysicalRay* RayPool::popRay()
{
    QMutexLocker locker(&m_mutex);
    if (m_rays.isEmpty()) {
        emit exhausted();
        return NULL;
    }
    if (m_rays.size() % 1000 == 0)
        qDebug() << "POP" << m_rays.size();
    return m_rays.pop();
}
