#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "core/tracer/scene.h"
#include "core/tracer/renderedimage.h"
#include "core/objects/pictureobject.h"
#include "core/objects/lensobject.h"
#include "core/stubs/pictureobject_stub.h"
#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include "core/models/model.h"
#include "core/models/cameramodel.h"

#include "core/stubs/camerastub.h"
#include "core/stubs/lensobjectstub.h"

#include "glwidget.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    connect(&Scene::Instance(), SIGNAL(renderingFinished()), this, SLOT(savePic()));
    GLWidget* glWidget = new GLWidget;
    QTableView* tableView = new QTableView;
    QListView* listView = new QListView;
    QTableView* tableViewCamera = new QTableView;
    QGridLayout* editor_layout = new QGridLayout;

    ui->tabWidget->widget(0)->setLayout(editor_layout);
    //ui->tabWidget->activateWindow(0);
    editor_layout->addWidget(tableView,0,0);
    editor_layout->addWidget(listView,1,0);
    editor_layout->addWidget(tableViewCamera,2,0);
    editor_layout->addWidget(glWidget,0,1,3,1);
    editor_layout->setColumnStretch(1,1);

    QImage image;
    image.load("google.png");
    Scene::Instance().addStubObject("Google", new PictureObjectStub(Point3D(0, 0, 0), Vector3D(0, -3, 0), Vector3D(0, 0, -1), image));
    Scene::Instance().addStubObject("pewpewpew", new PictureObjectStub(Point3D(0, 0, 0), Vector3D(0, -3, 0), Vector3D(0, 0, 1), image));

    Scene::Instance().addStubObject("GoogleLens", new LenseObjectStub(Point3D(2, 2, 2), Vector3D(0, -3, 0), Vector3D(0, 0, 1), image, image, 0.4, 1.23));

//    QImage lensimg;
//    lensimg.load("lens.png");
//    LensObject *obj = new LensObject(Point3D(1, -1, 0.5), Vector3D(0, -1, 0), Vector3D(0, 0, -1),
//                                     lensimg, lensimg, 0.2, 1.1);


//    Scene::Instance().addObject("Lens", obj);

    Scene::Instance().addCamera(new CameraStub(Point3D(1, -1, 2), Vector3D(0, 5, 0), Vector3D(0, 0, -3),
                                                             Point3D(5, 0, 0), QSize(500, 300)));
    Scene::Instance().createObjectsFromStubs();

    //Scene::Instance().addObject("Lens", PhysicalTrianglePolygon(Point3D(55, 60, 0), Point3D(45, 40, 10), Point3D(45, 40, -10)));

    // Test code...

    SceneModel *scene_model=new SceneModel;
    CameraModel *camera_model = new CameraModel(glWidget);
    Virtual3DObjectDelegate *pic_delegate = new Virtual3DObjectDelegate(tableView, glWidget);
    listView->setModel(scene_model);
    listView->setItemDelegate(pic_delegate);
    tableViewCamera->setModel(camera_model);
    tableViewCamera->resizeRowsToContents();
    tableViewCamera->resizeColumnsToContents();
}

void MainWindow::savePic() {
    QGraphicsScene* out_image = new QGraphicsScene;
    out_image->addPixmap(QPixmap::fromImage(RenderedImage::Instance().image()));
    ui->graphicsView->setScene(out_image);
    RenderedImage::Instance().image().save("result.png");
    QMessageBox::information(this, "No one gives a fuck", "Rendering finished");
}

MainWindow::~MainWindow()
{
    // TODO(kazeevn) properly destroy everything
    delete ui;
}

void MainWindow::on_renderButton_clicked()
{
    Scene::Instance().startRendering();
}
