#include "core/models/model.h"
#include "core/models/lensmodel.h"
#include "core/models/flatmirrormodel.h"
#include "core/models/thinlensmodel.h"

#include <QTableView>
#include <QtGlobal>

#include <typeinfo>

int SceneModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return Scene::Instance().stub_objects().length();
};

QVariant SceneModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole)
        return QVariant(Scene::Instance().stub_objects().at(index.row())->name());
    else
        return QVariant();

};

bool SceneModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row);
    delete Scene::Instance().stub_objects()[row];
    Scene::Instance().stub_objects().takeAt(row);
    endRemoveRows();
    return true;
}

void SceneModel::refresh() {
    reset();
    //emit dataChanged(this->createIndex(0,0), this->createIndex(this->rowCount(),0));
}

Qt::ItemFlags PictureModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
};

Qt::ItemFlags SceneModel::flags(const QModelIndex &index) const
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
        if (gl_widget != NULL)
            gl_widget->updateGL();
        return true;
    }
    return false;
};

int PictureModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
};

QVariant PictureModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::EditRole || role == Qt::DisplayRole) {
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
    if (role==Qt::ToolTipRole) {
        switch (index.row()){
        case 0:
            return "Upper Left rectangle point";
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
    }
}

QVariant PictureModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QWidget* Virtual3DObjectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return new QWidget(parent);
}

void Virtual3DObjectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);
}

void Virtual3DObjectDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    delete m_parent->model();
    PictureObjectStub* picture = dynamic_cast<PictureObjectStub*>(Scene::Instance().stub_objects()[index.row()]);
    if (picture) {
        m_parent->setModel(new PictureModel(picture, gl_widget));
        return;
    }
    LensObjectStub* lens = dynamic_cast<LensObjectStub*>(Scene::Instance().stub_objects()[index.row()]);
    if (lens) {
        m_parent->setModel(new LensModel(lens, gl_widget));
        return;
    }
    FlatMirrorObjectStub* mirror =dynamic_cast<FlatMirrorObjectStub*>(Scene::Instance().stub_objects()[index.row()]);
    if (mirror){
        m_parent->setModel(new FlatMirrorModel(mirror, gl_widget));
        return;
    }
    ThinLensObjectStub* thin_lens = dynamic_cast<ThinLensObjectStub*>(Scene::Instance().stub_objects()[index.row()]);
    if (thin_lens){
        m_parent->setModel(new ThinLensModel(thin_lens, gl_widget));
        return;
    }
        // TODO(kazeevn) What about an exception?
    m_parent->resizeRowsToContents();
    m_parent->resizeColumnsToContents();

}

bool PictureModel::setImage(const QImage &image)
{
    m_object->setImage(image);
    return true;
}
