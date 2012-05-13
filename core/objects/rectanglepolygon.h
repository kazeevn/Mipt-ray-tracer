#ifndef RECTANGLEPOLYGON_H
#define RECTANGLEPOLYGON_H

#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include "core/geometry/rectangle3d.h"
#include "core/geometry/transformationmatrix.h"
#include "core/objects/virtual3dobject.h"
#include "core/objects/trianglepolygon.h"

class RectanglePolygon : public Virtual3DObject
{
public:
    /* Точка - координаты верхнего левого угла картинки
     * На два вектора (желательно ортогональных) натягивается прямоугольник
     * Первый соответствует "ширине", второй - "высоте"
     */
    RectanglePolygon(const Point3D& point, const Vector3D& v1, const Vector3D& v2);

    Point3D* intercrossWithRay(const Ray3D &ray);
    virtual void processIntersection(const Ray3D &ray, const Point3D &point)
    {
        Q_UNUSED(ray)
        Q_UNUSED(point)
    }
    const Rectangle3D& getRectangle() const {return m_rectangle;}
protected:
    Rectangle3D m_rectangle;
    /* Прямоугольный полигон для удобства бьётся на два треугольных
     * Бьется по диагонали, которая не включает верхний левый угол
     */
    TrianglePolygon m_poly1, m_poly2;
    /* Матрица перехода к локальному базису прямоугольника*/
    TransformationMatrix m_matrix;
};

#endif // RECTANGLEPOLYGON_H
