#ifndef LENSMODEL_H
#define LENSMODEL_H
#include "core/objects/stubs/lensobjectstub.h"
#include <QAbstractTableModel>
#include <QGLWidget>

class LensModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    LensModel(LensObjectStub* object=0, QGLWidget *widget = 0, QObject *parent = 0) : QAbstractTableModel(parent) {m_object = object; gl_widget=widget;}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 5;}
    int columnCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 3;}
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    LensObjectStub* m_object;
    QGLWidget* gl_widget;
};
#endif // LENSMODEL_H
