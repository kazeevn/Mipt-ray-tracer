#include "core/models/cameramodel.h"

Qt::ItemFlags CameraModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool CameraModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole && index.column()<3) {
        switch (index.row()) {
        case 0:
            Scene::Instance().camera()->point_m()[index.column()]=value.toDouble();
            break;
        case 1:
            Scene::Instance().camera()->v1()[index.column()]=value.toDouble();
            break;
        case 2:
            Scene::Instance().camera()->v2()[index.column()]=value.toDouble();
            break;
        case 3:
            Scene::Instance().camera()->summit()[index.column()]=value.toDouble();
            break;
        case 4:
            switch (index.column()){
            case 0:
                Scene::Instance().camera()->size().setWidth(value.toDouble());
                break;
            case 1:
                Scene::Instance().camera()->size().setHeight(value.toDouble());
                break;
            default:
                return false;
            }
    }
        gl_widget->updateGL();
        return true;
    }
    return false;
}

int CameraModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant CameraModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
    switch (index.row()){
    case 0:
        return Scene::Instance().camera()->point_m()[index.column()];
    case 1:
       return Scene::Instance().camera()->v1()[index.column()];
    case 2:
        return Scene::Instance().camera()->v2()[index.column()];
    case 3:
        return Scene::Instance().camera()->summit()[index.column()];
    case 4:
        switch (index.column()) {
        case 0:
            return Scene::Instance().camera()->size().width();
        case 1:
            return Scene::Instance().camera()->size().height();
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }
    } else
        return QVariant();
}

QVariant CameraModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return "Summit";
        case 4:
            return "Size";
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
