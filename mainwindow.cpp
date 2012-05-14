#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemdialog.h"

#include "core/tracer/scene.h"
#include "core/tracer/renderedimage.h"
#include "core/objects/pictureobject.h"
#include "core/objects/lensobject.h"
#include "core/objects/thinlensobject.h"
#include "core/geometry/point3d.h"
#include "core/geometry/vector3d.h"
#include "core/models/model.h"
#include "core/models/cameramodel.h"
#include "core/objects/stubs/pictureobjectstub.h"
#include "core/objects/stubs/camerastub.h"
#include "core/objects/stubs/lensobjectstub.h"
#include "core/objects/stubs/thinlensobjectstub.h"
#include "core/objects/stubs/flatmirrorobjectstub.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
 #include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    /* UI STUFF */
    ui->setupUi(this);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, ui->edit);
    glWidget = new GLWidget(splitter);
    splitter->addWidget(ui->toolBox);
    splitter->addWidget(glWidget);
    QVBoxLayout* layout = new QVBoxLayout(ui->edit);
    layout->addWidget(splitter);
    /* Initialization */
    RenderedImage::init(QSize(0, 0));
    connect(&Scene::Instance(), SIGNAL(renderingFinished()), this, SLOT(showPic()));
    QImage image;
    image.load("google.png");
    Scene::Instance().addStubObject("Google", new PictureObjectStub(Point3D(0, 0, 0), Vector3D(0, -3, 0), Vector3D(0, 0, -1), image));
    Scene::Instance().addStubObject("pewpewpew", new PictureObjectStub(Point3D(0, 0, 0), Vector3D(-1, -3, 0), Vector3D(-1, 0, 1), image));
    QImage lensimg;
    lensimg.load("lens.png");
    //Scene::Instance().addStubObject("Cool lens", new LensObjectStub(Point3D(1, -1, 1), Vector3D(0, -1, 0), Vector3D(0, 0, -1), lensimg, lensimg, QSize(50, 50), 0.5, 3.5));
    QImage thinlensimg;
    thinlensimg.load("thinlens.png");
    Scene::Instance().addStubObject("Thin lens", new ThinLensObjectStub(Point3D(1, 0, 1), Vector3D(0, -1, 0), Vector3D(0, 0, -1), thinlensimg, -0.5));
    Scene::Instance().addStubObject("Flat mirror", new FlatMirrorObjectStub(Point3D(-1, -1, -2), Vector3D(0, -1, 0), Vector3D(2, 0, -1), thinlensimg, 0.9));
    Scene::Instance().addCamera(new CameraStub(Point3D(0, 2, 2), Vector3D(1, 0, -3), Vector3D(1, -5, 0),
                                               Point3D(5, 0, 3), QSize(300, 500)));

    /* Models */
    scene_model=new SceneModel;
    CameraModel *camera_model = new CameraModel(glWidget);
    Virtual3DObjectDelegate *pic_delegate = new Virtual3DObjectDelegate(ui->objectsTableView, glWidget);
    ui->objectsListView->setModel(scene_model);
    ui->objectsListView->setItemDelegate(pic_delegate);
    QItemSelectionModel *selectionModel = ui->objectsListView->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    ui->cameraTableView->setModel(camera_model);
    ui->cameraTableView->resizeRowsToContents();
    ui->cameraTableView->resizeColumnsToContents();
}

void MainWindow::addItem()
{
    AddItemDialog* add_dialog = new AddItemDialog();
    add_dialog->show();
}

void MainWindow::selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    foreach(const QModelIndex& index, newSelection.indexes())
        Scene::Instance().stub_objects()[index.row()]->select();
    foreach(const QModelIndex& index, oldSelection.indexes())
        Scene::Instance().stub_objects()[index.row()]->deselect();
    glWidget->updateGL();
}

void MainWindow::removeRow()
{
    foreach(const QModelIndex& index, ui->objectsListView->selectionModel()->selectedRows())
        scene_model->removeRow(index.row());
    glWidget->updateGL();
}

MainWindow::~MainWindow()
{
    // TODO(kazeevn) properly destroy everything
    delete ui;
}

void MainWindow::doRender()
{
    Scene::Instance().createObjectsFromStubs();
    Scene::Instance().startRendering();
}

void MainWindow::showPic() {
    QGraphicsScene* out_image = new QGraphicsScene;
    out_image->addPixmap(QPixmap::fromImage(RenderedImage::Instance().image()));
    ui->graphicsView->setScene(out_image);
    QMessageBox::information(this, "No one gives a fuck, but...", "Rendering finished!");
}

void MainWindow::savePic()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
        return;
    bool saved = RenderedImage::Instance().image().save(fileName);
    if (saved)
        QMessageBox::information(this, "Give it yourself, Igor!", QString("Saved image to '%1'").arg(fileName));
    else
        QMessageBox::information(this,"Error saving file", "Probably your filename is wrong.");
}

void MainWindow::loadScene()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load scene"), "", tr("Scene Files (*.scene)"));
    if (fileName.isEmpty())
        return;
    Scene::Instance().loadStubsFromFile(fileName);
    QMessageBox::information(this, tr("Scene loaded"), tr("Scene loaded!"));
}

void MainWindow::saveScene()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save scene"), "", tr("Scene Files (*.scene)"));
    if (fileName.isEmpty())
        return;
    Scene::Instance().saveStubsToFile(fileName);
}
