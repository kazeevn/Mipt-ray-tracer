#ifndef WORKEROBJECT_H
#define WORKEROBJECT_H

#include <QRunnable>
#include "core/tracer/raypool.h"
#include "core/tracer/scene.h"

class WorkerObject : public QRunnable
{
public:
    WorkerObject();
    void run();
};

#endif // WORKERTHREAD_H
