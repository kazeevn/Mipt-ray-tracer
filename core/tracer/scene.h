#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include "core/objects/virtual3dobject.h"
#include "core/geometry/ray3d.h"

// Singletone object
class Scene : public QObject
{
    Q_OBJECT
public:
    static Scene& Instance() {
        static Scene theInstance;
        return theInstance;
    }
    void addObject(Virtual3DObject* object);
    void traceRay(Ray3D *ray);
private:
    Scene(QObject *parent = 0);
    ~Scene();

    QList<Virtual3DObject*> m_objects;
};

#endif // SCENE_H
