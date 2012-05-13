#include "model.h"
#include<QTableView>

int SceneModel::rowCount(const QModelIndex &parent) const {
    return Scene::Instance().stub_objects().length();
};

QVariant SceneModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole)
        return QVariant(Scene::Instance().stub_objects().at(index.row())->name());
    else
        return QVariant();

};

Qt::ItemFlags PictureModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
};

bool PictureModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole && index.column()<3) {
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
    }
        return true;
    }
    return false;
};

int PictureModel::columnCount(const QModelIndex &parent) const {
    return 3;
};

QVariant PictureModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
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
    default:
        return QVariant();
    }
    } else
        return QVariant();
}

QVariant PictureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation==Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Point";
        case 1:
            return "HVector";
        case 2:
            return "VVector";
        }
    } else if (orientation==Qt::Vertical){
        switch (section) {
        case 0:
            return "x";
        case 1:
            return "y";
        case 2:
            return "z";
        }
    }

}

QWidget* PictureDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTableView* editor = new QTableView(parent);
    return editor;
}
/*
void PictureDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
 //   editor->setModel(new PictureModel(Scene::Instance().stub_objects())
}
*/
