#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QSize>
#include <QThreadPool>
#include "core/objects/virtual3dobject.h"
#include "core/stubs/virtual3dobject_stub.h"
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
    void addObject(const QString& name, Virtual3DObject* object);
    void traceRay(Ray3D *ray);
    void startRendering(const Point3D& cameraPos, const Rectangle3D& screen, const QSize& picsize);
    const QList<Virtual3DObject*>& objects() const {return m_objects;}
    // Stub
    const QList<Virtual3DObjectStub*>& stub_objects() const {return m_stubs;}
    void addStubObject(const QString& name, Virtual3DObjectStub* object);

    void createObjectsFromStubs();
signals:
    void renderingFinished();
private:
    Scene(QObject *parent = 0);
    ~Scene();

    RenderingHelper* m_renderingHelper;
    QList<Virtual3DObject*> m_objects;
    // Stub
    QList<Virtual3DObjectStub*> m_stubs;
};

#endif // SCENE_H
