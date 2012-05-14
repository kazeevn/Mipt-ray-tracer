#include "lensobject.h"

#include <QColor>
#include <QDebug>

LensObject::LensObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &heightMap1, const QImage &heightMap2, double height, double refractiveIndex)
    : m_rectangle(point, v1, v2), m_matrix(v1, v2, v2.crossProduct(v1).unit()),
      m_heightMap1(heightMap1), m_heightMap2(heightMap2),
      m_height(height), m_refractiveIndex(refractiveIndex),
      m_perpendicular(v2.crossProduct(v1).unit()*height),

      m_frontPolygon(point+m_perpendicular, v1, v2),
      m_backPolygon(point-m_perpendicular, v2, v1),
      m_leftPolygon(point-m_perpendicular, m_perpendicular*2, v2),
      m_rightPolygon(point-m_perpendicular+v1, v2, m_perpendicular*2),
      m_topPolygon(point-m_perpendicular, v1, m_perpendicular*2),
      m_bottomPolygon(point-m_perpendicular+v2, m_perpendicular*2, v1),

      m_frontSize(heightMap1.size().width()+1, heightMap1.size().height()+1),
      m_backSize(heightMap2.size().width()+1, heightMap2.size().height()+1),
      m_frontPolygons(m_frontSize.width(), m_frontSize.height()),
      m_backPolygons(m_backSize.width(), m_backSize.height())

{
    triangulateSurfaces();
}

LensObject::~LensObject()
{
}

double LensObject::getFrontHeight(int i, int j)
{
    double value = 0;
    if ((i > 0) && (i < m_frontSize.width()) && (j > 0) && (j < m_frontSize.height())) {
        QColor c = QColor(m_heightMap1.pixel(i-1, j-1));
        value = c.blackF();
    }
    return value;
}

double LensObject::getBackHeight(int i, int j)
{
    double value = 0;
    if ((i > 0) && (i < m_backSize.width()) && (j > 0) && (j < m_backSize.height())) {
        QColor c = QColor(m_heightMap2.pixel(i-1, j-1));
        value = c.blackF();
    }
    return value;
}

Point3D LensObject::frontPoint(int i, int j)
{
    return m_rectangle.point()+m_rectangle.horizontalVect()*(double(i)/m_frontSize.width())
                              +m_rectangle.verticalVect()*(double(j)/m_frontSize.height())
                              +m_perpendicular*getFrontHeight(i, j);
}

Point3D LensObject::backPoint(int i, int j)
{
    return m_rectangle.point()+m_rectangle.horizontalVect()*(double(i)/m_backSize.width())
                              +m_rectangle.verticalVect()*(double(j)/m_backSize.height())
                              +m_perpendicular*getBackHeight(i, j);
}

void LensObject::triangulateSurfaces()
{
    for (int i = 0; i < m_frontSize.width(); i++) {
        for (int j = 0; j < m_frontSize.height(); j++) {
            m_frontPolygons.at(i, j, 0) = new PhysicalTrianglePolygon(frontPoint(i, j),
                                                                      frontPoint(i+1, j),
                                                                      frontPoint(i, j+1));
            m_frontPolygons.at(i, j, 1) = new PhysicalTrianglePolygon(frontPoint(i+1, j),
                                                                      frontPoint(i+1, j+1),
                                                                      frontPoint(i, j+1));
        }
    }

    for (int i = 0; i < m_backSize.width(); i++) {
        for (int j = 0; j < m_backSize.height(); j++) {
            m_backPolygons.at(i, j, 0) = new PhysicalTrianglePolygon(backPoint(i, j),
                                                                      backPoint(i+1, j),
                                                                      backPoint(i, j+1));
            m_backPolygons.at(i, j, 1) = new PhysicalTrianglePolygon(backPoint(i+1, j),
                                                                      backPoint(i+1, j+1),
                                                                      backPoint(i, j+1));
        }
    }
}

Point3D* LensObject::intercrossWithRay(const Ray3D &ray)
{
    Point3D* p1 = 0;
    p1 = m_frontPolygon.intercrossWithRay(ray);
    p1 = (p1 == 0) ? m_backPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_leftPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_rightPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_topPolygon.intercrossWithRay(ray) : p1;
    p1 = (p1 == 0) ? m_bottomPolygon.intercrossWithRay(ray) : p1;
    if (p1 == NULL)
        return NULL;
    delete p1;

    // После эвристик - честный пробег по всем полигонам
    Point3D* minpoint = 0;
    double mindist = -1;

    for (int i = 0; i < m_frontSize.width(); i++)
        for (int j = 0; j < m_frontSize.height(); j++)
            for (int k = 0; k < 2; k++)
                if ((p1 = m_frontPolygons.at(i, j, k)->intercrossWithRay(ray)) != NULL) {
                    if ((minpoint == NULL) || (p1->dist(ray.point()) < mindist)) {
                        minpoint = p1;
                        mindist = p1->dist(ray.point());
                    } else
                        delete p1;
                }
    for (int i = 0; i < m_backSize.width(); i++)
        for (int j = 0; j < m_backSize.height(); j++)
            for (int k = 0; k < 2; k++)
                if ((p1 = m_backPolygons.at(i, j, k)->intercrossWithRay(ray)) != NULL) {
                    if ((minpoint == NULL) || (p1->dist(ray.point()) < mindist)) {
                        minpoint = p1;
                        mindist = p1->dist(ray.point());
                    } else
                        delete p1;
                }
    return minpoint;
}

void LensObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
    Q_UNUSED(ray);
    Vector3D oldvector(m_rectangle.point(), point);
    // На самом деле это трехмерный вектор, с координатами от 0 до 1 по (x, y)
    // И с высотой над поверхностью по z
    Vector3D newvector = m_matrix.transformVector(oldvector);
    Point3D* helperPoint = 0;
    PhysicalTrianglePolygon* helperPolygon = 0;
    if (newvector.z > 0) {
        int coordx = newvector.x*m_frontSize.width();
        int coordy = newvector.y*m_frontSize.height();
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                for (int k = 0; k < 2; k++) {
                    if ((coordx+i >= 0) && (coordx+i < m_frontSize.width()) && \
                            (coordy+j >= 0) && (coordy+j < m_frontSize.height()) && \
                            ((helperPolygon = m_frontPolygons.at(coordx+i, coordy+j, k)) != NULL)) {
                        helperPoint = helperPolygon->intercrossWithRay(ray);
                        if (helperPoint) {
                            delete helperPoint;
                            helperPolygon->processPhysicalIntersection(ray, point, m_refractiveIndex);
                        }
                    }
                }
    } else {
        int coordx = newvector.x*m_backSize.width();
        int coordy = newvector.y*m_backSize.height();
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                for (int k = 0; k < 2; k++)
                    if ((coordx+i >= 0) && (coordx+i < m_backSize.width()) && \
                            (coordy+j >= 0) && (coordy+j < m_backSize.height()) && \
                            ((helperPolygon = m_backPolygons.at(coordx+i, coordy+j, k)) != NULL)) {
                        helperPoint = helperPolygon->intercrossWithRay(ray);
                        if (helperPoint) {
                            delete helperPoint;
                            helperPolygon->processPhysicalIntersection(ray, point, m_refractiveIndex);
                        }
                    }

    }
}

void LensObject::setRefractiveIndex(double refractiveIndex)
{
    m_refractiveIndex = refractiveIndex;
}
