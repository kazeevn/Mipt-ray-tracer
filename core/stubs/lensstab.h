#ifndef LENSSTAB_H
#define LENSSTAB_H
#include"core/stubs/pictureobject_stub.h"
class LensStab : public PictureObjectStub
{
public:
    LensStab(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const QImage &image, const double n) :
        PictureObjectStub(point, v1, v2, image), refractiveIndex(n) {};
    double refractiveIndex;
};

#endif // LENSSTAB_H
