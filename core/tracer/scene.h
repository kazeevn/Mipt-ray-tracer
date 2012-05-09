#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include <QSize>
#include <QThreadPool>
#include "core/objects/virtual3dobject.h"
#include "core/geometry/ray3d.h"
#include "core/tracer/renderinghelper.h"

// Singletone object
class Scene : public QObject
{
    Q_OBJECT
public:
    static Scene& Instance() {
        static Scene theInstance;
        return theInstance;
    }
    void addObject(Virtual3DObject* object);
    void traceRay(Ray3D *ray);
    void startRendering(const Point3D& cameraPos, const Rectangle3D& screen, const QSize& picsize);
signals:
    void renderingFinished();
private:
    Scene(QObject *parent = 0);
    ~Scene();

    RenderingHelper* m_renderingHelper;

    QList<Virtual3DObject*> m_objects;
};

#endif // SCENE_H
