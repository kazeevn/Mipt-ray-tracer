#ifndef FLATMIRROROBJECTSTUB_H
#define FLATMIRROROBJECTSTUB_H

#include "core/objects/stubs/virtual3dobjectstub.h"
#include <QImage>

class FlatMirrorObjectStub : public Virtual3DObjectStub
{
public:
    FlatMirrorObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &bitmask, double reflcoef);
    void setBitmask(const QImage &bitmask) { m_bitmask = bitmask; }
    QImage& bitmask() { return m_bitmask; }
    double& reflcoef() { return m_reflcoef; }

    bool isValid();
private:
    QImage m_bitmask;
    double m_reflcoef;
};

#endif // FLATMIRROROBJECTSTUB_H
