#ifndef TRIANGLEPOLYGONSURFACE_H
#define TRIANGLEPOLYGONSURFACE_H

#include <cstring>
#include <QDebug>

template <typename T>
class TrianglePolygonSurface
{
public:
    TrianglePolygonSurface(int hsize, int vsize)
        : m_hsize(hsize), m_vsize(vsize)
    {
        m_polygons = new T*[hsize*vsize*2];
        memset(m_polygons, 0, hsize*vsize*2*sizeof(T*));
    }

    ~TrianglePolygonSurface()
    {
        for (int i = 0; i < m_hsize; i++)
            for (int j = 0; j < m_vsize; j++)
                for (int k = 0; k < 2; k++)
                    if (at(i, j, k) != NULL)
                        delete at(i, j, k);
        delete[] m_polygons;
    }

    bool inside(int x, int y, int k)
    {
        return ((x >= 0) && (x < m_hsize) && (y >= 0) && (y < m_vsize) && (k >= 0) && (k < 2));
    }

    T*& at(int x, int y, int k)
    {
        return m_polygons[x*m_vsize*2 + y*2 + k];
    }

private:
    T** m_polygons;
    int m_hsize;
    int m_vsize;
};

#endif // TRIANGLEPOLYGONSURFACE_H
