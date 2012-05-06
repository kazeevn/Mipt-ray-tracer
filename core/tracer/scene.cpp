#include "scene.h"
#include "core/geometry/point3d.h"

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

void Scene::traceRay(const Ray3D& ray)
{
    Point3D *point = 0, *minpoint = 0;
    Virtual3DObject *nearestObj = 0;
    double mindist = 0;
    // Searching for nearest collision with any object
    Q_FOREACH(Virtual3DObject* obj, m_objects)
    {
        if ((point = obj->intercrossWithRay(ray)) != NULL) {
            double dist = point->dist(ray.getp());
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
        nearestObj->processIntersection(ray, *point);
        delete point;
    }
    // TODO: in other case, ray came to infinity
    // Should process this case somehow too
}
