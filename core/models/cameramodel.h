#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H

#include "core/tracer/scene.h"
#include "core/stubs/camerastub.h"
#include <QAbstractTableModel>
#include <QTableView>
#include <QGLWidget>
#include <QtGlobal>
#include <QObject>

class CameraModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CameraModel(QGLWidget *widget = 0, QObject *parent = 0) : QAbstractTableModel(parent) {gl_widget=widget;}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const {Q_UNUSED(parent); return 5;}
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QGLWidget* gl_widget;
};

#endif // CAMERAMODEL_H
