#include "renderedimage.h"
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
    /* Средневзвешенное цветов упершихся лучей с весами в интенсивность */
    QColor prevcolor(m_image.pixel(physray->startingX(), physray->startingY()));
    QColor deltaColor(color);
    QColor newcolor = QColor::fromRgbF(prevcolor.redF()+deltaColor.redF()*physray->intensity(),
                                       prevcolor.greenF()+deltaColor.greenF()*physray->intensity(),
                                       prevcolor.blueF()+deltaColor.blueF()*physray->intensity());
/*    qDebug() << "Point" << physray->startingX() << physray->startingY() << \
                "Switched from" << prevcolor.name() << "TO" << newcolor.name() << \
                "because of" << physray->intensity() << QColor(color).name();*/
    m_image.setPixel(physray->startingX(), physray->startingY(),
                     newcolor.rgb());
}
