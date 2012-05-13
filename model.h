#ifndef MODEL_H
#define MODEL_H
#include "core/tracer/scene.h"
#include "core/stubs/pictureobject_stub.h"
#include <QAbstractListModel>
#include<QItemDelegate>
#include<QTableView>
#include<QGLWidget>

class SceneModel : public QAbstractListModel
{
        Q_OBJECT
public:
    SceneModel(QObject *parent = 0) : QAbstractListModel(parent) {}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role) { return true;}
};

class PictureDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PictureDelegate(QObject *parent = 0, QGLWidget *draw_me=0) {m_parent=(QTableView*)parent; gl_widget=draw_me;}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
private:
    QTableView* m_parent;
    QGLWidget* gl_widget;
};

class PictureModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PictureModel(PictureObjectStub* object=0, QGLWidget *widget = 0, QObject *parent = 0) : QAbstractTableModel(parent) {m_object = object; gl_widget=widget;}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const {return 4;}
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    PictureObjectStub* m_object;
    QGLWidget* gl_widget;
};

#endif // MODEL_H
