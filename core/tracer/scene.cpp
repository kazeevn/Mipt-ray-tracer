#include "scene.h"

#include <QColor>
#include <QThreadPool>
#include "core/geometry/point3d.h"
#include "core/tracer/renderedimage.h"
#include "core/tracer/physicalray.h"
#include "core/objects/pictureobject.h"
#include "core/stubs/pictureobject_stub.h"

Scene::Scene(QObject *parent) :
    QObject(parent),
    m_renderingHelper(0),
    m_objects()
{
}

Scene::~Scene()
{
    qDeleteAll(m_objects);
    m_objects.clear();
}

void Scene::addObject(const QString& name, Virtual3DObject *object)
{
    object->setName(name);
    m_objects.append(object);
}

void Scene::addStubObject(const QString &name, Virtual3DObjectStub *object)
{
    object->setName(name);
    m_stubs.append(object);
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
    if (m_renderingHelper) delete m_renderingHelper;
    m_renderingHelper = new RenderingHelper(cameraPos, screen, picsize);
    connect(m_renderingHelper, SIGNAL(renderingFinished()), this, SIGNAL(renderingFinished()));
    QThreadPool::globalInstance()->start(m_renderingHelper);
}


void Scene::createObjectsFromStubs()
{
    Q_FOREACH(Virtual3DObjectStub* obj, m_stubs)
    {
        PictureObjectStub *pstub = dynamic_cast<PictureObjectStub*>(obj);
        if (!pstub)
            addObject(pstub->name(), new PictureObject(pstub->point(), pstub->horizontalVect(), pstub->verticalVect(), pstub->image()));
    }
}
