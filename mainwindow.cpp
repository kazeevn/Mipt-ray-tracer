#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "core/tracer/scene.h"
#include "core/tracer/renderedimage.h"
#include "core/objects/pictureobject.h"
#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include "core/geometry/rectangle3d.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    QImage image;
    image.load("google.png");
    Point3D p(1, 0, 0);
    Vector3D v1(1, 3, 1);
    Vector3D v2(0, 0, 1);

    Scene::Instance().addObject(new PictureObject(image, p, v1, v2));

    Scene::Instance().startRendering(Point3D(0, 0, 0),
                                     Rectangle3D(Point3D(1, -1, -1), Vector3D(0, 5, 0), Vector3D(0, 0, 3)),
                                     QSize(500, 300));
}

void MainWindow::savePic() {
    qDebug() << "SAVE";
    RenderedImage::Instance().image().save("result.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}
