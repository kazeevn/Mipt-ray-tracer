#ifndef FLATMIRRORMODEL_H
#define FLATMIRRORMODEL_H

#include "core/objects/stubs/flatmirrorobjectstub.h"
#include <QAbstractTableModel>
#include <QGLWidget>

class FlatMirrorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    FlatMirrorModel(FlatMirrorObjectStub* object=0, QGLWidget *widget = 0, QObject *parent = 0) : QAbstractTableModel(parent) {m_object = object; gl_widget=widget;}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 4;}
    int columnCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 3;}
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setImage(const QImage& image);
private:
    FlatMirrorObjectStub* m_object;
    QGLWidget* gl_widget;
};
#endif // FLATMIRRORMODEL_H
