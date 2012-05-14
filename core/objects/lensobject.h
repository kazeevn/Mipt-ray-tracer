#ifndef LENSOBJECT_H
#define LENSOBJECT_H

#include "core/objects/virtual3dobject.h"
#include "core/objects/rectanglepolygon.h"
#include "core/objects/physicalpolygons.h"
#include "core/geometry/rectangle3d.h"
#include "core/geometry/ray3d.h"
#include "core/geometry/point3d.h"
#include "core/geometry/transformationmatrix.h"

#include <QImage>

/*
 * Класс, отвечающий за физическую линзу
 * Положение задается как Rectangle3D
 * Профиль задается картами высот heightMap{1,2}, причем карта высот
 * с индексом 1 отвечает стороне, соответствующей векторныму произведению [v2; v1]
 * (а второй - соответственно противоположной)
 * Черный цвет на карте высот отвечает нулевой толщине, белый - максимальной
 * Толщина задается параметром height
 */
class LensObject : public Virtual3DObject
{
public:
    LensObject(const Point3D& point, const Vector3D& v1, const Vector3D& v2,
               const QImage &heightMap1, const QImage &heightMap2,
               double height, double refractiveIndex);

    Point3D* intercrossWithRay(const Ray3D &ray);
    void processIntersection(const Ray3D &ray, const Point3D &point);

    void setRefractiveIndex(double refractiveIndex);

    Point3D frontPoint(int i, int j);
    Point3D backPoint(int i, int j);
    const QSize& frontSize() { return m_frontSize; }
    const QSize& backSize() { return m_backSize; }
private:
    double getFrontHeight(int i, int j);
    double getBackHeight(int i, int j);

    void triangulateSurfaces();

    Rectangle3D m_rectangle;
    TransformationMatrix m_matrix;
    QImage m_heightMap1, m_heightMap2;
    double m_height, m_refractiveIndex;
    Vector3D m_perpendicular;
    /* Для эвристик, погружаем сперва линзу в прямоугольный паралелепипед с этими полигонами */
    RectanglePolygon m_frontPolygon, m_backPolygon, m_leftPolygon, m_rightPolygon, m_topPolygon, m_bottomPolygon;

    // Количество полигонов! Количество точек - эта хрень плюс один; количество пикселей - эта хрень минус один
    QSize m_frontSize, m_backSize;
    Vector3D m_frontdx, m_frontdy, m_frontdz, m_backdx, m_backdy, m_backdz;
    PhysicalTetragonPolygon ***m_frontPolygons, ***m_backPolygons;
};

#endif // LENSOBJECT_H
