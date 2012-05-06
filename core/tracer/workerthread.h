#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "core/tracer/raypool.h"
#include "core/tracer/scene.h"

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(QObject *parent = 0);
    void start();
signals:
    
public slots:
    
};

#endif // WORKERTHREAD_H
