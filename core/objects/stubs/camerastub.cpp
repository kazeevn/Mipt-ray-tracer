#include "camerastub.h"

#include <cfloat>

bool CameraStub::set_size(QSize size)
{
    m_size=size;
    return true;
}

bool CameraStub::isValid()
{
    if (!Virtual3DObjectStub::isValid())
        return false;
    if (Vector3D(summit(), point()).dotProduct(v1().crossProduct(v2())) < DBL_EPSILON)
        return false;
    return true;
}
