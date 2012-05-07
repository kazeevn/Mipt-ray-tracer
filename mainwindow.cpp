#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "core/tracer/scene.h"
#include "core/tracer/renderedimage.h"
#include "core/objects/pictureobject.h"
#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    QImage image;
    image.load("google.png");
    Point3D p(1, 0, 0);
    Vector3D v1(0, 3, 0);
    Vector3D v2(0, 0, 1);

    RenderedImage::init(QSize(100, 100));

    Scene::Instance().addObject(new PictureObject(image, p, v1, v2));
    Scene::Instance().traceRay(new Ray3D(0, 0, 0, 1, 0.1, 0.5));
    Scene::Instance().traceRay(new Ray3D(0, 0, 0, -1, 1.5, 0.5));
}

MainWindow::~MainWindow()
{
    delete ui;
}
