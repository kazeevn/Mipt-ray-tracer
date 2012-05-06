#ifndef TRANSFORMATIONMATRIX_H
#define TRANSFORMATIONMATRIX_H

#include "core/geometry/vector3d.h"

class TransformationMatrix
{
public:
    /* Строит матрицу перехода к базису из векторов v1, v2, v3
     * То есть: умножая её на вектор в исходном базисе, мы получим его-же
     * представление в новом базисе. По столбцам у нее - координаты орт исходного базиса
     * в новом. */
    TransformationMatrix(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3);
    Vector3D transformVector(const Vector3D& v);
private:
    double m_matrix[3][3];
};

#endif // TRANSFORMATIONMATRIX_H
