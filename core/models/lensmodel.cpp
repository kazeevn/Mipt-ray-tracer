#include "core/models/lensmodel.h"

Qt::ItemFlags LensModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
};

bool LensModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole && index.column()<4) {
        switch (index.row()) {
        case 0:
            m_object->point_m()[index.column()]=value.toDouble();
            break;
        case 1:
            m_object->v1()[index.column()]=value.toDouble();
            break;
        case 2:
            m_object->v2()[index.column()]=value.toDouble();
            break;
        case 3:
            if (index.column()==0)
                m_object->height()=value.toDouble();
            else
                return false;
            break;
        case 4:
            if (index.column()==0)
                m_object->refractiveIndex()=value.toDouble();
            else
                return false;
            break;
        }
        gl_widget->updateGL();
        return true;
    }
    return false;
};

QVariant LensModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch (index.row()){
    case 0:
        return m_object->point_m()[index.column()];
        break;
    case 1:
       return m_object->v1()[index.column()];
       break;
    case 2:
        return m_object->v2()[index.column()];
        break;
    case 3:
        if (index.column()==0)
            return m_object->height();
        else
            return QVariant();
    case 4:
        if (index.column()==0)
            return m_object->refractiveIndex();
        else
            return QVariant();
    default:
        return QVariant();
    }
    } else
        return QVariant();
}

QVariant LensModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation==Qt::Vertical) {
        switch (section) {
        case 0:
            return "Point";
        case 1:
            return "HVector";
        case 2:
            return "VVector";
        case 3:
            return "height";
        case 4:
            return "n";
        }
    } else if (orientation==Qt::Horizontal){
        switch (section) {
        case 0:
            return "x";
        case 1:
            return "y";
        case 2:
            return "z";
        }
    }
    return QVariant();
}

bool LensModel::setFrontHeightMap(const QImage &image)
{
    m_object->heightMap1()=image;
    return true;
}

bool LensModel::setBackHeightMap(const QImage &image)
{
    m_object->heightMap2()=image;
    return true;
}
