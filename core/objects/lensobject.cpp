#include "lensobject.h"

#include <QColor>
#include <QDebug>

LensObject::LensObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &heightMap1, const QImage &heightMap2, double height, double refractiveIndex)
    : m_rectangle(point, v1, v2), m_matrix(v1, v2, v1.crossProduct(v2).unit()),
      m_heightMap1(heightMap1), m_heightMap2(heightMap2),
      m_height(height), m_refractiveIndex(refractiveIndex),
      m_perpendicular(v2.crossProduct(v1).unit()*height),

      m_frontPolygon(point+m_perpendicular, v1, v2),
      m_frontLeftPolygon(point, v2, m_perpendicular),
      m_frontRightPolygon(point+v1, v2, m_perpendicular),
      m_frontTopPolygon(point, m_perpendicular, v1),
      m_frontBottomPolygon(point+v2, v1, m_perpendicular),

      m_backPolygon(point-m_perpendicular, v2, v1),
      m_backLeftPolygon(point, m_perpendicular*(-1), v2),
      m_backRightPolygon(point+v1, v2, m_perpendicular*(-1)),
      m_backTopPolygon(point, v1, m_perpendicular*(-1)),
      m_backBottomPolygon(point+v2, m_perpendicular*(-1), v1),

      m_frontSize(heightMap1.size().width()+1, heightMap1.size().height()+1),
      m_backSize(heightMap2.size().width()+1, heightMap2.size().height()+1)
{
    triangulateSurfaces();
}

void LensObject::triangulateSurfaces()
{
    Vector3D frontdx(m_rectangle.horizontalVect() / m_frontSize.width());
    Vector3D frontdy(m_rectangle.verticalVect() / m_frontSize.height());
    Vector3D frontdz(m_perpendicular.unit());
    m_frontPolygons = new PhysicalTetragonPolygon**[m_frontSize.width()];
    for (int i = 0; i < m_frontSize.width(); i++) {
        m_frontPolygons[i] = new PhysicalTetragonPolygon*[m_frontSize.height()];
        for (int j = 0; j < m_frontSize.height(); j++) {
            Point3D curpoint = m_rectangle.point() + frontdx*i + frontdy*j;
            m_frontPolygons[i][j] = new PhysicalTetragonPolygon(curpoint+frontdz*getFrontHeight(i, j),
                                                                curpoint+frontdx+frontdz*getFrontHeight(i+1, j),
                                                                curpoint+frontdx+frontdy+frontdz*getFrontHeight(i+1, j+1),
                                                                curpoint+frontdy+frontdz*getFrontHeight(i, j+1));
        }
    }

    Vector3D backdx(m_rectangle.horizontalVect() / m_backSize.width());
    Vector3D backdy(m_rectangle.verticalVect() / m_backSize.height());
    Vector3D backdz(m_perpendicular.unit()*(-1));
    m_backPolygons = new PhysicalTetragonPolygon**[m_heightMap2.size().width()];
    for (int i = 0; i < m_backSize.width(); i++) {
        m_backPolygons[i] = new PhysicalTetragonPolygon*[m_heightMap2.size().height()];
        for (int j = 0; j < m_backSize.height(); j++) {
            Point3D curpoint = m_rectangle.point() + backdx*i + backdy*j;
            m_backPolygons[i][j] = new PhysicalTetragonPolygon(curpoint+backdz*getBackHeight(i, j),
                                                               curpoint+backdx+backdz*getBackHeight(i+1, j),
                                                               curpoint+backdx+backdy+backdz*getBackHeight(i+1, j+1),
                                                               curpoint+backdy+backdz*getBackHeight(i, j+1));
        }
    }
}

Point3D* LensObject::intercrossWithRay(const Ray3D &ray)
{
    Point3D* p1 = 0;
    p1 = m_frontPolygon.intercrossWithRay(ray);
    p1 = (p1 == 0) ? m_frontLeftPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_frontRightPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_frontTopPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_frontBottomPolygon.intercrossWithRay(ray) : p1;

    Point3D* p2 = 0;
    p2 = m_backPolygon.intercrossWithRay(ray);
    p2 = (p2 == 0) ? m_backLeftPolygon.intercrossWithRay(ray) : p2;
    p2 = (p2 == 0) ? m_backRightPolygon.intercrossWithRay(ray) : p2;
    p2 = (p2 == 0) ? m_backTopPolygon.intercrossWithRay(ray) : p2;
    p2 = (p2 == 0) ? m_backBottomPolygon.intercrossWithRay(ray) : p2;

    bool back = false, front = false;

    if ((p1 != NULL) && (p2 != NULL)) {
        if (p1->dist(ray.point()) > p2->dist(ray.point()))
            back = true;
        else
            front = true;
        delete p1;
        delete p2;
    } else if (p1 != NULL) {
        delete p1;
        front = true;
    } else if (p2 != NULL) {
        delete p2;
        back = true;
    }
    // После эвристик - честный пробег по всем полигонам
    if (front) {
        for (int i = 0; i < m_frontSize.width(); i++)
            for (int j = 0; j < m_frontSize.height(); j++)
                if ((p1 = m_frontPolygons[i][j]->intercrossWithRay(ray)) != NULL)
                    return p1;
    } else if (back) {
        for (int i = 0; i < m_backSize.width(); i++)
            for (int j = 0; j < m_backSize.height(); j++)
                if ((p2 = m_backPolygons[i][j]->intercrossWithRay(ray)) != NULL)
                    return p2;
    }

    return NULL;
}

double LensObject::getFrontHeight(int i, int j)
{
    if (i == 0 || i == m_frontSize.width() || j == 0 || j == m_frontSize.height())
        return 0;
    else
        return (QColor(m_heightMap1.pixel(i-1, j-1)).blackF()+1) / 255 * m_height;
}

double LensObject::getBackHeight(int i, int j)
{
    if (i == 0 || i == m_backSize.width() || j == 0 || j == m_backSize.height())
        return 0;
    else
        return (QColor(m_heightMap2.pixel(i-1, j-1)).blackF()+1) / 255 * m_height;
}

void LensObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
    Q_UNUSED(ray);
    Vector3D oldvector(m_rectangle.point(), point);
    // На самом деле это трехмерный вектор, с координатами от 0 до 1 по (x, y)
    // И с высотой над поверхностью по z
    Vector3D newvector = m_matrix.transformVector(oldvector);
    if (newvector.z > 0) {
        int coordx = newvector.x*m_frontSize.width();
        int coordy = newvector.y*m_frontSize.height();
        Point3D *helper = 0;
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if ((coordx+i >= 0) && (coordx+i < m_frontSize.width()) && \
                        (coordy+j >= 0) && (coordy+j < m_frontSize.height())) {
                    helper = m_frontPolygons[coordx+i][coordy+j]->intercrossWithRay(ray);
                    if (helper) {
                        delete helper;
                        m_frontPolygons[coordx+i][coordy+j]->processPhysicalIntersection(ray, point, m_refractiveIndex);
                    }
                }
    } else {
        int coordx = newvector.x*m_backSize.width();
        int coordy = newvector.y*m_backSize.height();
        Point3D *helper = 0;
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if ((coordx+i >= 0) && (coordx+i < m_backSize.width()) && \
                        (coordy+j >= 0) && (coordy < m_backSize.height())) {
                    helper = m_backPolygons[coordx+i][coordy+j]->intercrossWithRay(ray);
                    if (helper) {
                        delete helper;
                        m_backPolygons[coordx+i][coordy+j]->processPhysicalIntersection(ray, point, m_refractiveIndex);
                    }
                }
    }
}

void LensObject::setRefractiveIndex(double refractiveIndex)
{
    m_refractiveIndex = refractiveIndex;
}
