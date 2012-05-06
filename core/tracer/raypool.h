#ifndef RAYPOOL_H
#define RAYPOOL_H

#include <QObject>
#include <QList>
#include "core/geometry/ray3d.h"

// Singletone object
class RayPool : public QObject
{
    Q_OBJECT
public:
    static RayPool& Instance() {
        static RayPool theInstance;
        return theInstance;
    }
    explicit RayPool(QObject *parent = 0);

    void pushRay(Ray3D* ray);
    Ray3D* popRay();
private:
    QList<Ray3D*> m_rays;
};

#endif // RAYPOOL_H
