#include "workerobject.h"


WorkerObject::WorkerObject()
    : QRunnable()
{
}

void WorkerObject::run()
{
    Ray3D* curRay = 0;
    while ((curRay = RayPool::Instance().popRay()) != NULL) {
        Scene::Instance().traceRay(curRay);
        delete curRay;
    }
}
