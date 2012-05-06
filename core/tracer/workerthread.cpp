#include "workerthread.h"

WorkerThread::WorkerThread(QObject *parent) :
    QThread(parent)
{
}

void WorkerThread::start()
{
    Ray3D* curRay = 0;
    while (curRay = RayPool::Instance().popRay()) {
        Scene::Instance().traceRay(*curRay);
        delete curRay;
    }
}
