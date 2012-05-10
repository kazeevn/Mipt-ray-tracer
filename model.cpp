#include "model.h"


int SceneModel::rowCount(const QModelIndex &parent) const {
    return Scene::Instance().length();
};

QVariant SceneModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole)
        return QVariant(Scene::Instance().ListObjects().at(index.row())->name);
    else
        return QVariant();

};
