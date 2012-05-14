#ifndef CAMERASTUB_H
#define CAMERASTUB_H
#include "core/objects/stubs/virtual3dobjectstub.h"
#include <QSize>

class CameraStub : public Virtual3DObjectStub
{
public:
    // Firs the rectangle, than summit
    CameraStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const Point3D& camera_point, const QSize& size)
        : Virtual3DObjectStub(point, v1, v2), m_summit(camera_point),m_size(size) {}
    Point3D& summit() {return m_summit;}
    QSize& size() {return m_size;}
    bool set_size(QSize size);

    bool isValid();
private:
    Point3D m_summit;
    QSize m_size;
};

#endif // CAMERASTUB_H
