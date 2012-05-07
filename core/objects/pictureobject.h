#ifndef PICTUREOBJECT_H
#define PICTUREOBJECT_H

#include <QImage>
#include "core/objects/virtual3dobject.h"
#include "core/geometry/vector3d.h"
#include "core/geometry/point3d.h"
#include "core/geometry/rectangle3d.h"
#include "core/objects/trianglepolygon.h"
#include "core/geometry/transformationmatrix.h"

class PictureObject : public Virtual3DObject
{
public:
    /* Положение задается как у Rectangle3D */
    PictureObject(const QImage &image, const Point3D& point, const Vector3D& v1, const Vector3D& v2);

    Point3D* intercrossWithRay(const Ray3D &ray);
    void processIntersection(const Ray3D &ray, const Point3D &point);
private:
    QImage m_image;
    Rectangle3D m_rectangle;
    /* Прямоугольная картинка для удобства бьётся на два треугольных полигона
     * Бьется по диагонали, которая не включает нижний левый угол
     */
    TrianglePolygon m_poly1, m_poly2;
    /* Матрица перехода к локальному базису картинки */
    TransformationMatrix m_matrix;
};

#endif // PICTUREOBJECT_H
