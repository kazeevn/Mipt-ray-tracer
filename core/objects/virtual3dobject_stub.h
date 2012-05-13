#ifndef VIRTUAL3DOBJECT_STUB_H
#define VIRTUAL3DOBJECT_STUB_H
#include"core/geometry/rectangle3d.h"
#include <QString>
#include <QList>

class Virtual3DObjectStub : public Rectangle3D
{
public:
    Virtual3DObjectStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2) : Rectangle3D(point, v1, v2) {};
    const QString& name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    QList<Point3D> getPoints();

private:
    QString m_name;
};

#endif // VIRTUAL3DOBJECT_STUB_H
