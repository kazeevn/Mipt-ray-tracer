#include "renderedimage.h"
#include <QDebug>
#include <QMutexLocker>
#include "core/tracer/physicalray.h"

RenderedImage *RenderedImage::theInstance = 0;

RenderedImage::RenderedImage(QSize size)
    : m_mutex(),
      m_image(size, QImage::Format_RGB32)

{
    m_image.fill(Qt::black);
}

void RenderedImage::rayTraceResult(const Ray3D &ray, QRgb color)
{
    QMutexLocker locker(m_mutex);
    qDebug() << "GOT RAY TRACE RESULT";
    const PhysicalRay* physray = dynamic_cast<const PhysicalRay*>(&ray);
    if (!physray)
        return;
    /* Средневзвешенное цветов упершихся лучей с весами в интенсивность */
    m_image.setPixel(physray->startingX(), physray->startingY(),
                     physray->intensity()*color \
                     + m_image.pixel(physray->startingX(), physray->startingY()));
}
