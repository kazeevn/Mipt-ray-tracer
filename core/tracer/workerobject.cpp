#include "workerobject.h"
#include <QDebug>
#include <QThread>


WorkerObject::WorkerObject()
    : QRunnable()
{
    qDebug() << "Worker object" << QThread::currentThreadId() << "created";
}

void WorkerObject::run()
{
    qDebug() << "Worker object" << QThread::currentThreadId() << "is ready to work";
    PhysicalRay* curRay = 0;
    while ((curRay = RayPool::Instance().popRay()) != NULL) {
        Scene::Instance().traceRay(curRay);
        delete curRay;
    }
    qDebug() << "Worker object" << QThread::currentThreadId() << "finished working";
}
