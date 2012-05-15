#ifndef PHYSICALRAY_H
#define PHYSICALRAY_H

#include "core/geometry/ray3d.h"

/*
 * "Физический" луч. Несет в себе информацию об: интенсивности
 * (число от 0 до 1 по отношению к исходному испущенному лучу);
 * точке, откуда был выпущен исходный луч (два числа - stx, sty -
 * позиция на отрендеренной картинке, для которой этот луч несет
 * информацию); ВОЗМОЖНО: поляризации
 */
class PhysicalRay : public Ray3D
{
public:
    PhysicalRay(const Point3D &p, const Vector3D &v,
                const int stx, const int sty,
                const int reflcount,
                const double intensity);
    PhysicalRay(const Point3D &p1, const Point3D &p2,
                const int stx, const int sty,
                const int reflcount,
                const double intensity);
    int startingX() const { return m_stx; }
    int startingY() const { return m_sty; }
    double intensity() const { return m_intensity; }
    int reflcount() const { return m_reflcount; }
private:
    int m_stx, m_sty;
    int m_reflcount;
    double m_intensity;
};

#endif // PHYSICALRAY_H
