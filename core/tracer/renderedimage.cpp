#include "renderedimage.h"

#include <QMutexLocker>
#include <QColor>

RenderedImage *RenderedImage::theInstance = 0;

RenderedImage::RenderedImage(QSize size)
    : m_mutex(),
      m_image(size, QImage::Format_RGB32)

{
    m_image.fill(Qt::black);
}

void RenderedImage::rayTraceResult(const PhysicalRay &ray, QRgb color)
{
    QMutexLocker locker(&m_mutex);
    /* Средневзвешенное цветов упершихся лучей с весами в интенсивность */
    QColor prevcolor(m_image.pixel(ray.startingX(), ray.startingY()));
    QColor deltaColor(color);
    QColor newcolor = QColor::fromRgbF(qMin(prevcolor.redF()+deltaColor.redF()*ray.intensity(), 1.0),
                                       qMin(prevcolor.greenF()+deltaColor.greenF()*ray.intensity(), 1.0),
                                       qMin(prevcolor.blueF()+deltaColor.blueF()*ray.intensity(), 1.0));
    /*    qDebug() << "Point" << ray.startingX() << ray.startingY() << \
                "Switched from" << prevcolor.name() << "TO" << newcolor.name() << \
                "because of" << ray.intensity() << QColor(color).name();*/
    m_image.setPixel(ray.startingX(), ray.startingY(),
                     newcolor.rgb());
}
