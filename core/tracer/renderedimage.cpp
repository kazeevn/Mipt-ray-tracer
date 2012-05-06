#include "renderedimage.h"

RenderedImage *RenderedImage::theInstance;

RenderedImage::RenderedImage(QSize size)
    : m_image(size, QImage::Format_RGB32)
{
}

void RenderedImage::rayTraceResult(const Ray3D &ray, QRgb color)
{
    // TODO: наполнить картинку
}
