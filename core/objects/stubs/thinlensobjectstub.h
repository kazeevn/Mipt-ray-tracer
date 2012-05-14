#ifndef THINLENSOBJECTSTUB_H
#define THINLENSOBJECTSTUB_H

#include "core/objects/stubs/virtual3dobjectstub.h"

#include <QImage>

class ThinLensObjectStub : public Virtual3DObjectStub
{
public:
    ThinLensObjectStub(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &bitmask, double focus);
    void setBitmask(const QImage &bitmask) { m_bitmask = bitmask; }
    QImage& bitmask() { return m_bitmask; }
    double& focus() { return m_focus; }

    bool isValid();
private:
    QImage m_bitmask;
    double m_focus;
};

#endif // THINLENSOBJECTSTUB_H
