#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "core/tracer/scene.h"
#include "core/tracer/renderedimage.h"
#include "core/objects/pictureobject.h"
#include "core/objects/lensobject.h"
#include "core/stubs/pictureobject_stub.h"
#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include "model.h"
#include "glwidget.h"

#include <QMessageBox>

#include <QStandardItemModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    connect(&Scene::Instance(), SIGNAL(renderingFinished()), this, SLOT(savePic()));
    GLWidget* glWidget = new GLWidget;
    ui->formLayout->addWidget(glWidget);

    QImage image;
    image.load("google.png");
    Scene::Instance().addStubObject("Google", new PictureObjectStub(Point3D(0, 0, 0), Vector3D(0, -3, 0), Vector3D(0, 0, -1), image));
    Scene::Instance().addStubObject("pewpewpew", new PictureObjectStub(Point3D(0, 0, 0), Vector3D(0, -3, 0), Vector3D(0, 0, 1), image));


    Scene::Instance().createObjectsFromStubs();

   //Scene::Instance().addObject("Lens", PhysicalTrianglePolygon(Point3D(55, 60, 0), Point3D(45, 40, 10), Point3D(45, 40, -10)));

    // Test code...
    // QImage lensimg;
    // lensimg.load("lens.png");
    // LensObject *obj = new LensObject(Point3D(-1, 1, 0), Vector3D(2, 0, 0), Vector3D(0, -2, 0),
    //                                     lensimg, lensimg, 0.5, 1.0);

    SceneModel *scene_model=new SceneModel;
    PictureDelegate *pic_delegate = new PictureDelegate(ui->tableView, glWidget);
    ui->listView->setModel(scene_model);
    ui->listView->setItemDelegate(pic_delegate);   
}

void MainWindow::savePic() {
    QGraphicsScene* out_image = new QGraphicsScene;
    out_image->addPixmap(QPixmap::fromImage(RenderedImage::Instance().image()));
    ui->graphicsView->setScene(out_image);
//    RenderedImage::Instance().image().save("result.png");
    QMessageBox::information(this, "No one gives a fuck", "Rendering finished");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    Scene::Instance().startRendering(Point3D(0, 0, 0),
                                         Rectangle3D(Point3D(1, -1, 2), Vector3D(0, 5, 0), Vector3D(0, 0, -3)),
                                         QSize(500, 300));
}
