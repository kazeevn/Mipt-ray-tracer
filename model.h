#ifndef MODEL_H
#define MODEL_H
#include "core/tracer/scene.h"
 #include <QAbstractListModel>

class SceneModel : public QAbstractListModel
{
        Q_OBJECT
public:
    SceneModel(QObject *parent = 0) : QAbstractListModel(parent) {};
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
};

#endif // MODEL_H
