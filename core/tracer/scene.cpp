#include "scene.h"

#include "core/geometry/point3d.h"
#include "core/tracer/renderedimage.h"
#include "core/tracer/physicalray.h"
#include "core/tracer/raypool.h"
#include "core/tracer/workerobject.h"
#include "core/objects/pictureobject.h"
#include <QColor>
#include <QDebug>

Scene::Scene(QObject *parent) :
    QObject(parent),
    m_objects(),
    m_workerThreadPool(this)
{
    m_workerThreadPool.setMaxThreadCount(1);
}

Scene::~Scene()
{
    qDeleteAll(m_objects);
    m_objects.clear();
}

void Scene::addObject(Virtual3DObject *object)
{
    m_objects.append(object);
}

void Scene::traceRay(Ray3D *ray)
{
    Point3D *point = 0, *minpoint = 0;
    Virtual3DObject *nearestObj = 0;
    double mindist = 0;
    // Searching for nearest collision with any object
    Q_FOREACH(Virtual3DObject* obj, m_objects)
    {
        PictureObject* picobj = dynamic_cast<PictureObject*>(obj);
        if (picobj)
            point = picobj->intercrossWithRay(*ray);
        else
            point = NULL;
        if (point != NULL) {
            double dist = point->dist(ray->point());
            if ((minpoint == NULL) || (dist < mindist)) {
                if (minpoint != NULL)
                    delete minpoint;
                minpoint = point;
                mindist = dist;
                nearestObj = obj;
            } else
                delete point;
        }
    }
    if (nearestObj) {
        nearestObj->processIntersection(*ray, *minpoint);
        delete minpoint;
    } else
        RenderedImage::Instance().rayTraceResult(*ray, QColor(Qt::black).rgb());
}

void Scene::startRendering(const Point3D &cameraPos, const Rectangle3D &screen, const QSize &picsize)
{
    RenderedImage::Instance().init(picsize);

    Vector3D dx = screen.horizontalVect() / picsize.width();
    Vector3D dy = screen.verticalVect() / picsize.height();
    for (int i = 0; i < picsize.width(); i++)
        for (int j = 0; j < picsize.height(); j++)
            RayPool::Instance().pushRay(new PhysicalRay(cameraPos, screen.point()+dx*i+dy*j, i, j, 1.0f));

    for (int i = 0; i < m_workerThreadPool.maxThreadCount(); i++)
        m_workerThreadPool.start(new WorkerObject());
}
