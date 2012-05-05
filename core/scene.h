#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include "core/virtual3dobject.h"
#include "core/ray3d.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();
    void addObject(Virtual3DObject* object);
    void traceRay(const Ray3D &ray);
private:
    QList<Virtual3DObject*> m_objects;
};

#endif // SCENE_H
