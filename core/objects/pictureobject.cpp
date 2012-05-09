#include "pictureobject.h"
#include "core/tracer/renderedimage.h"

PictureObject::PictureObject(const Point3D &point, const Vector3D &v1, const Vector3D &v2, const QImage &image)
    : RectanglePolygon(point, v1, v2),
      m_image(image),
      m_matrix(v1, v2, v1.crossProduct(v2))
{
}

void PictureObject::processIntersection(const Ray3D &ray, const Point3D &point)
{
    Q_UNUSED(ray);
    Vector3D oldvector(m_rectangle.point(), point);
    // На самом деле это двумерный вектор, с координатами от 0 до 1
    // где точка (0, 0) - нижняя левая для картинки, а (1, 0) - нижняя правая
    Vector3D newvector = m_matrix.transformVector(oldvector);
    QRgb color = m_image.pixel(newvector.x*m_image.width(),
                               newvector.y*m_image.height());
    RenderedImage::Instance().rayTraceResult(ray, color);
}


