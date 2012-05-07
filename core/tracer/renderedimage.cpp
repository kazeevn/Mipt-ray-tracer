#include "renderedimage.h"
#include <QDebug>
#include <QColor>

RenderedImage *RenderedImage::theInstance = 0;

RenderedImage::RenderedImage(QSize size)
    : m_image(size, QImage::Format_RGB32)
{
}

void RenderedImage::rayTraceResult(const Ray3D &ray, QRgb color)
{
    qDebug() << "Got ray trace result!" << QColor::fromRgb(color).name();
    // TODO: наполнить картинку
}
