#include "renderinghelper.h"
#include "core/tracer/physicalray.h"
#include "core/tracer/raypool.h"
#include "core/tracer/workerobject.h"
#include "core/tracer/renderedimage.h"


RenderingHelper::RenderingHelper(const Point3D &cameraPos, const Rectangle3D &screen, const QSize &picsize, QObject *parent)
    : QObject(parent), m_cameraPos(cameraPos), m_screen(screen), m_size(picsize), m_workerThreadPool()
{
    m_workerThreadPool.setMaxThreadCount(4);
}

void RenderingHelper::run()
{
    RenderedImage::Instance().init(m_size);

    Vector3D dx = m_screen.horizontalVect() / m_size.width();
    Vector3D dy = m_screen.verticalVect() / m_size.height();
    for (int i = 0; i < m_size.width(); i++)
        for (int j = 0; j < m_size.height(); j++)
            RayPool::Instance().pushRay(new PhysicalRay(m_cameraPos, m_screen.point()+dx*i+dy*j, i, j, 1.0f));

    for (int i = 0; i < m_workerThreadPool.maxThreadCount(); i++)
        m_workerThreadPool.start(new WorkerObject());
    m_workerThreadPool.waitForDone();
    emit renderingFinished();
}
