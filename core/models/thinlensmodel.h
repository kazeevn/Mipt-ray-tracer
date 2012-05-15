#ifndef THINLENSMODEL_H
#define THINLENSMODEL_H

#include "core/objects/stubs/thinlensobjectstub.h"
#include <QAbstractTableModel>
#include <QGLWidget>

class ThinLensModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ThinLensModel(ThinLensObjectStub* object=0, QGLWidget *widget = 0, QObject *parent = 0) : QAbstractTableModel(parent) {m_object = object; gl_widget=widget;}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 4;}
    int columnCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 3;}
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    ThinLensObjectStub* m_object;
    QGLWidget* gl_widget;
};

#endif // THINLENSMODEL_H
