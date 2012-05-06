#ifndef RAYPOOL_H
#define RAYPOOL_H

#include <QObject>
#include <QList>
#include "core/geometry/ray3d.h"

// Singletone object
// XXX: thread safety! (mutex, etc)
class RayPool : public QObject
{
    Q_OBJECT
public:
    static RayPool& Instance() {
        static RayPool theInstance;
        return theInstance;
    }

    void pushRay(Ray3D* ray);
    Ray3D* popRay();
private:
    RayPool(QObject *parent = 0);
    QList<Ray3D*> m_rays;
};

#endif // RAYPOOL_H
