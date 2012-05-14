#include "raypool.h"
#include <QMutexLocker>

RayPool::RayPool(QObject *parent)
    : QObject(parent),
      m_rays(),
      m_mutex(),
      m_totalCount(0)
{
}

void RayPool::pushRay(PhysicalRay *ray)
{
    QMutexLocker locker(&m_mutex);
    if (ray->intensity() < 4e-3)
        return;
    m_totalCount++;
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
        qDebug() << "POP" << m_rays.size() << m_totalCount;
    return m_rays.pop();
}
