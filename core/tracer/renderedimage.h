#ifndef RENDEREDIMAGE_H
#define RENDEREDIMAGE_H

#include <QImage>
#include "core/geometry/ray3d.h"

/* TODO: thread-safety
 * Класс, собирающий информацию о всех лучах, пришедших куда-либо откуда-либо */

class RenderedImage
{
public:
    static void init(QSize size) {
        theInstance = new RenderedImage(size);
    }

    static RenderedImage& Instance() {
        return *theInstance;
    }

    void rayTraceResult(const Ray3D& ray, QRgb color);
private:
    static RenderedImage *theInstance;

    RenderedImage(QSize size);
    QImage m_image;
};


#endif // RENDEREDIMAGE_H
