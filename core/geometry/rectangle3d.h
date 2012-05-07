#ifndef RECTANGLE3D_H
#define RECTANGLE3D_H

#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"

class Rectangle3D
{
public:
    /* Точка - координаты нижнего левого угла картинки
     * На два вектора (желательно ортогональных) натягивается прямоугольник
     * Первый соответствует "ширине", второй - "высоте"
     */
    Rectangle3D(const Point3D& p, const Vector3D& v1, const Vector3D& v2);
    const Point3D& point() const { return m_point; }
    const Vector3D& horizontalVect() const { return m_v1; }
    const Vector3D& verticalVect() const { return m_v2; }
private:
    Point3D m_point;
    Vector3D m_v1;
    Vector3D m_v2;
};

#endif // RECTANGLE3D_H
