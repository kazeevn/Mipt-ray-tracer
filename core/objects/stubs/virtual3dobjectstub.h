#ifndef VIRTUAL3DOBJECT_STUB_H
#define VIRTUAL3DOBJECT_STUB_H
#include"core/geometry/rectangle3d.h"
#include <QString>
#include <QList>

class Virtual3DObjectStub : public Rectangle3D
{    
public:
    Virtual3DObjectStub(const Point3D& point, const Vector3D& v1, const Vector3D& v2) : Rectangle3D(point, v1, v2) {m_selected=false;}
    virtual ~Virtual3DObjectStub() {}
    const QString& name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    QList<Point3D> getPoints();
    bool selected() {return m_selected;}
    void select() {m_selected=true;}
    void deselect() {m_selected=false;}

    virtual bool isValid();
private:
    QString m_name;
    bool m_selected;
};

#endif // VIRTUAL3DOBJECT_STUB_H
