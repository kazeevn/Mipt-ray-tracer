#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QtOpenGL>
#include <QList>
#include <QWheelEvent>

#include <math.h>

#include "core/stubs/pictureobject_stub.h"
#include "core/tracer/scene.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

/*
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
*/
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QPoint lastPos;
    float r; //Radius of camera
    float speed; //Camera sensetivity
    float teta;
    float phi; //coords of camera

    //Private methods
    void drawGround( float fExtent, float fStep, float y);
    void drawCamera();
    void gluPerspective(double fovy,double aspect, double zNear, double zFar);
    void rotateCamera();
};

#endif // GLWIDGET_H
