#ifndef PICTUREOBJECT_H
#define PICTUREOBJECT_H

#include <QImage>
#include "core/objects/virtual3dobject.h"
#include "core/geometry/vector3d.h"
#include "core/geometry/point3d.h"
#include "core/objects/trianglepolygon.h"
#include "core/geometry/transformationmatrix.h"

class PictureObject : public Virtual3DObject
{
public:
    /* Точка - координаты нижнего левого угла картинки
     * На два вектора (желательно ортогональных) натягивается картинка
     * Первый соответствует "ширине", второй - "высоте"
     */
    PictureObject(QImage &image, Point3D &point, Vector3D &v1, Vector3D &v2);

    Point3D* intercrossWithRay(const Ray3D &ray);
    void processIntersection(const Ray3D &ray, const Point3D &point);
private:
    QImage& m_image;
    Point3D m_point;
    Vector3D m_v1, m_v2;
    /* Прямоугольная картинка для удобства бьётся на два треугольных полигона
     * Бьется по диагонали, которая не включает нижний левый угол
     */
    TrianglePolygon m_poly1, m_poly2;
    /* Матрица перехода к локальному базису картинки */
    TransformationMatrix m_matrix;
};

#endif // PICTUREOBJECT_H
