#include "renderedimage.h"
#include <QDebug>
#include <QMutexLocker>
#include "core/tracer/physicalray.h"
#include <QColor>

RenderedImage *RenderedImage::theInstance = 0;

RenderedImage::RenderedImage(QSize size)
    : m_mutex(),
      m_image(size, QImage::Format_RGB32)

{
    m_image.fill(Qt::black);
}

void RenderedImage::rayTraceResult(const Ray3D &ray, QRgb color)
{
    QMutexLocker locker(&m_mutex);
    const PhysicalRay* physray = dynamic_cast<const PhysicalRay*>(&ray);
    if (!physray)
        return;
    qDebug() << "Got ray trace result" << QColor::fromRgb(color).name() << "to point" << physray->startingX() << physray->startingY();
    qDebug() << "NEW COLOR:" << QColor::fromRgb(physray->intensity()*color + m_image.pixel(physray->startingX(), physray->startingY())).name();
    /* Средневзвешенное цветов упершихся лучей с весами в интенсивность */
    m_image.setPixel(physray->startingX(), physray->startingY(),
                     physray->intensity()*color \
                     + m_image.pixel(physray->startingX(), physray->startingY()));
}
