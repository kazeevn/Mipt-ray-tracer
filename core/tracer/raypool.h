#ifndef RAYPOOL_H
#define RAYPOOL_H

#include <QObject>
#include <QStack>
#include <QMutex>
#include "core/tracer/physicalray.h"

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

    void pushRay(PhysicalRay* ray);
    PhysicalRay *popRay();
signals:
    void exhausted();
private:
    RayPool(QObject *parent = 0);
    QStack<PhysicalRay*> m_rays;
    QMutex m_mutex;
    int m_totalCount;
};

#endif // RAYPOOL_H
