#include "model.h"


int SceneModel::rowCount(const QModelIndex &parent) const {
    return Scene::Instance().objects().length();
};

QVariant SceneModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole)
        return QVariant(Scene::Instance().objects().at(index.row())->name());
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
    if (index.isValid() && role == Qt::EditRole) {

        return true;
    }
    return false;
};

int PictureModel::columnCount(const QModelIndex &parent) const {
    if (0 <= parent.row() <= 2)
        return 3;
    else if (parent.row()==4)
        return 1;
    else
        return 0;

};

QVariant PictureModel::data(const QModelIndex &index, int role) const {
    return QVariant(false);
}
