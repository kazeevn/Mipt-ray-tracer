#include "transformationmatrix.h"

TransformationMatrix::TransformationMatrix(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3)
{
    /*
     * Inverse matrix to:
     * v1.x  v1.y  v1.z
     * v2.x  v2.y  v2.z
     * v3.x  v3.y  v3.z
     */
    double det = v1.x*v2.y*v3.z + v2.x*v3.y*v1.z + v3.x*v1.y*v2.z \
            - v1.z*v2.y*v3.x - v1.y*v2.x*v3.z - v1.x*v2.z*v3.y;
    m_matrix[0][0] = (v2.y*v3.z - v2.z*v3.y)/det;
    m_matrix[0][1] = (v2.z*v3.x - v3.z*v2.x)/det;
    m_matrix[0][2] = (v2.x*v3.y - v2.y*v3.x)/det;
    m_matrix[1][0] = (v1.z*v3.y - v1.y*v3.z)/det;
    m_matrix[1][1] = (v1.x*v3.z - v1.z*v3.x)/det;
    m_matrix[1][2] = (v1.y*v3.x - v1.x*v3.y)/det;
    m_matrix[2][0] = (v1.y*v2.z - v1.z*v2.y)/det;
    m_matrix[2][1] = (v1.z*v2.x - v1.x*v2.z)/det;
    m_matrix[2][2] = (v1.x*v2.y - v1.y*v2.x)/det;
}

Vector3D TransformationMatrix::transformVector(const Vector3D &v)
{
    Vector3D result(m_matrix[0][0]*v.x + m_matrix[0][1]*v.y + m_matrix[0][2]*v.z,
                    m_matrix[1][0]*v.x + m_matrix[1][1]*v.y + m_matrix[1][2]*v.z,
                    m_matrix[2][0]*v.x + m_matrix[2][1]*v.y + m_matrix[2][2]*v.z);
    return result;
}
