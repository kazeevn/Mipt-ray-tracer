#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "core/tracer/scene.h"
#include "core/tracer/renderedimage.h"
#include "core/objects/pictureobject.h"
#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include "model.h"

#include <QMessageBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    connect(&Scene::Instance(), SIGNAL(renderingFinished()), this, SLOT(savePic()));

    QImage image;
    image.load("google.png");
    Scene::Instance().addObject(new PictureObject(image, Point3D(1, 0, 1), Vector3D(1, 10, -1), Vector3D(0, 0, -1)));
    Scene::Instance().addObject(new PictureObject(image, Point3D(1, 4, 1), Vector3D(0, -3, 0), Vector3D(0, 0, -1)));

    SceneModel *model=new SceneModel;
    //QMessageBox::information(this,"dd",QString("%1").arg(model.rowCount()));
    ui->listView->setModel(model);
    /*Scene::Instance().startRendering(Point3D(0, 0, 0),
                                     Rectangle3D(Point3D(1, -1, 2), Vector3D(0, 5, 0), Vector3D(0, 0, -3)),
                                     QSize(500, 300));*/
}

void MainWindow::savePic() {
    RenderedImage::Instance().image().save("result.png");
    QMessageBox::information(this, "Rendering finished", "Saved result to 'result.png'");
}

MainWindow::~MainWindow()
{
    delete ui;
}
