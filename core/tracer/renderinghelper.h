#ifndef RENDERINGHELPER_H
#define RENDERINGHELPER_H

#include <QObject>
#include <QRunnable>
#include <QSize>
#include <QThreadPool>
#include "core/geometry/point3d.h"
#include "core/geometry/rectangle3d.h"

class RenderingHelper : public QObject, public QRunnable
{
    Q_OBJECT
public:
    RenderingHelper(const Point3D &cameraPos, const Rectangle3D &screen, const QSize &picsize, QObject *parent = 0);
    ~RenderingHelper();

    void run();
signals:
    void renderingFinished();
private:
    Point3D m_cameraPos;
    Rectangle3D m_screen;
    QSize m_size;
    QThreadPool m_workerThreadPool;

    static int THREAD_COUNT;
    static int RAYS_PER_DOT;
};

#endif // RENDERINGHELPER_H
