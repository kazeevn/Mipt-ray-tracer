#ifndef RAYPOOL_H
#define RAYPOOL_H

#include <QObject>
#include <QList>
#include <QMutex>
#include "core/geometry/ray3d.h"

/* Пул лучей, откуда WorkerObject'ы достают лучи чтобы их оттрассировать
 * и куда объекты добавляют новые (в случае, если в результате взаимодействия
 * с объектом возник новый луч - как, например, с линзой) */
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
signals:
    void exhausted();
private:
    RayPool(QObject *parent = 0);
    QList<Ray3D*> m_rays;
    QMutex m_mutex;
};

#endif // RAYPOOL_H
