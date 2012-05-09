#ifndef PICTUREOBJECT_H
#define PICTUREOBJECT_H

#include <QImage>
#include "core/objects/virtual3dobject.h"
#include "core/objects/rectanglepolygon.h"
#include "core/geometry/vector3d.h"
#include "core/geometry/point3d.h"
#include "core/geometry/transformationmatrix.h"

class PictureObject : public RectanglePolygon
{
public:
    /* Положение задается как у Rectangle3D */
    PictureObject(const Point3D& point, const Vector3D& v1, const Vector3D& v2, const QImage &image);

    void processIntersection(const Ray3D &ray, const Point3D &point);
private:
    QImage m_image;
    /* Матрица перехода к локальному базису картинки */
    TransformationMatrix m_matrix;
};

#endif // PICTUREOBJECT_H
