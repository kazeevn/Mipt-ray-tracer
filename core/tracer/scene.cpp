#include "scene.h"

#include "core/geometry/point3d.h"
#include "core/tracer/renderedimage.h"

#include "core/objects/pictureobject.h"
#include <QColor>
#include <QDebug>

Scene::Scene(QObject *parent) :
    QObject(parent),
    m_objects()
{
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
            double dist = point->dist(ray->getp());
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
