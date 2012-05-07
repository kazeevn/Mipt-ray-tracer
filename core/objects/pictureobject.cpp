#include "pictureobject.h"
#include "core/tracer/renderedimage.h"

PictureObject::PictureObject(QImage &image, Point3D &point, Vector3D &v1, Vector3D &v2)
    : m_image(image),
      m_point(point), m_v1(v1), m_v2(v2),
      m_poly1(point, point+v1, point+v2),
      m_poly2(point+v1, point+v2, point+v1+v2),
      m_matrix(v1, v2, v1.crossProduct(v2))
{
}

Point3D* PictureObject::intercrossWithRay(const Ray3D &ray) {
    Point3D* p = m_poly1.intercrossWithRay(ray);
    if (p) return p;
    p = m_poly2.intercrossWithRay(ray);
    return p;
}

void PictureObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
    Q_UNUSED(ray);
    Vector3D oldvector(m_point, point);
    // На самом деле это двумерный вектор, с координатами от 0 до 1
    // где точка (0, 0) - нижняя левая для картинки, а (1, 0) - нижняя правая
    Vector3D newvector = m_matrix.transformVector(oldvector);
    QRgb color = m_image.pixel(newvector.x*m_image.width(),
                               newvector.y*m_image.height());
    RenderedImage::Instance().rayTraceResult(ray, color);
}


