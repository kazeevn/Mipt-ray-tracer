#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include "core/virtual3dobject.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();
    void addObject(Virtual3DObject* object);
private:
    QList<Virtual3DObject*> m_objects;
};

#endif // SCENE_H
