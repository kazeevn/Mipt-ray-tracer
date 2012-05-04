#include "scene.h"

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
