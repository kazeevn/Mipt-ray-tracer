#ifndef MODEL_H
#define MODEL_H
#include "core/tracer/scene.h"
#include "core/objects/pictureobject.h"
 #include <QAbstractListModel>

class SceneModel : public QAbstractListModel
{
        Q_OBJECT
public:
    SceneModel(QObject *parent = 0) : QAbstractListModel(parent) {};
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
};

class PictureModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PictureModel(PictureObject* object=0, QObject *parent = 0) : QAbstractTableModel(parent) {p_object = object;}
    int rowCount( const QModelIndex & parent = QModelIndex() ) const {return 4;}
    int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
private:
    PictureObject* p_object;
};

#endif // MODEL_H
