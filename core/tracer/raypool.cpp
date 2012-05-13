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
    PhysicalRay* pray = dynamic_cast<PhysicalRay*>(ray);
    if (pray && (pray->intensity() < 1e-3))
        return;
    m_rays.append(ray);
}

Ray3D* RayPool::popRay()
{
    QMutexLocker locker(&m_mutex);
    if (m_rays.isEmpty()) {
        emit exhausted();
        return NULL;
    }
    if (m_rays.length() % 1000 == 0)
        qDebug() << "POP" << m_rays.length();
    return m_rays.takeFirst();
}
