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

RenderingHelper::~RenderingHelper()
{
}

void RenderingHelper::run()
{
    for (int i = 0; i < m_size.width(); i++)
        for (int j = 0; j < m_size.height(); j++)
            RayPool::Instance().pushRay(new PhysicalRay(m_cameraPos,
                                                        m_screen.point()+(m_screen.horizontalVect())*(double(i) / (m_size.width()-1))+
                                                                         (m_screen.verticalVect())*(double(j) / (m_size.height()-1)),
                                                        i, j, 1.0f));

    for (int i = 0; i < m_workerThreadPool.maxThreadCount(); i++)
        m_workerThreadPool.start(new WorkerObject());
    m_workerThreadPool.waitForDone();
    emit renderingFinished();
}
