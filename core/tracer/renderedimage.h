#ifndef RENDEREDIMAGE_H
#define RENDEREDIMAGE_H

#include <QImage>
#include <QSize>
#include <QMutex>
#include "core/geometry/ray3d.h"

/* Класс, собирающий информацию о всех лучах, пришедших куда-либо откуда-либо */
class RenderedImage
{
public:
    static void init(QSize size) {
        if (theInstance)
            delete theInstance;
        theInstance = new RenderedImage(size);
    }

    static RenderedImage& Instance() {
        return *theInstance;
    }

    const QSize size() const { return m_image.size(); }
    const QImage image() const { return m_image; }

    void rayTraceResult(const Ray3D &ray, QRgb color);
private:
    RenderedImage(QSize size);

    static RenderedImage *theInstance;

    QMutex m_mutex;
    QImage m_image;
};


#endif // RENDEREDIMAGE_H
