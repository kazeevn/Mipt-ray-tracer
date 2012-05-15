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
#include "core/models/lensmodel.h"
#include "core/models/flatmirrormodel.h"
#include "core/models/thinlensmodel.h"

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

//    QImage image;
//    image.load("picture.png");
//    Scene::Instance().addStubObject("Google", new PictureObjectStub(Point3D(0, 2, 2), Vector3D(0, -4, 0), Vector3D(0, 0, -4), image));
//    QImage gplus;
//    gplus.load("sphere.png");
//    Scene::Instance().addStubObject("sphere ball", new LensObjectStub(Point3D(2, 1, 1), Vector3D(0, -1, 0), Vector3D(0, 0, -1), gplus, gplus, QSize(50, 50), 0.5, 1.5));
//    qDebug() << "DAFUQ";
    //QImage thinlensimg;
    //thinlensimg.load("thinlens.png");
    Scene::Instance().addCamera(new CameraStub(Point3D(0, 2, 2), Vector3D(0, 0, -3), Vector3D(0, -3, 0),
                                               Point3D(5, 0, 0), QSize(500, 500)));

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
    AddItemDialog* add_dialog = new AddItemDialog(this);
    add_dialog->show();    
}

void MainWindow::selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    foreach(const QModelIndex& index, newSelection.indexes())
        Scene::Instance().stub_objects()[index.row()]->select();
    foreach(const QModelIndex& index, oldSelection.indexes())
        Scene::Instance().stub_objects()[index.row()]->deselect();
    LensModel* lens_model = dynamic_cast<LensModel*>(ui->objectsTableView->model());
    if (lens_model)
        ui->loadImageBack->setEnabled(true);
    else
        ui->loadImageBack->setEnabled(false);
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
    ui->renderButton->setEnabled(false);
    Scene::Instance().createObjectsFromStubs();
    Scene::Instance().startRendering();
}

void MainWindow::showPic() {
    ui->renderButton->setEnabled(true);
    QGraphicsScene* out_image = new QGraphicsScene;
    out_image->addPixmap(QPixmap::fromImage(RenderedImage::Instance().image()));
    ui->graphicsView->setScene(out_image);
    QMessageBox::information(this, "Information", "Rendering finished!");
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
    this->refresh();
}

void MainWindow::saveScene()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save scene"), "", tr("Scene Files (*.scene)"));
    if (fileName.isEmpty())
        return;
    Scene::Instance().saveStubsToFile(fileName);
}

void MainWindow::on_loadImageFront_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    PictureModel* pic_model = dynamic_cast<PictureModel*>(ui->objectsTableView->model());
    if (pic_model) {
        pic_model->setImage(QImage(fileName));
        return;
    }
    LensModel* lens_model = dynamic_cast<LensModel*>(ui->objectsTableView->model());
    if (lens_model) {
        lens_model->setFrontHeightMap(QImage(fileName));
        return;
    }
    ThinLensModel* thin_lens_model = dynamic_cast<ThinLensModel*>(ui->objectsTableView->model());
    if (thin_lens_model) {
        thin_lens_model->setImage(QImage(fileName));
        return;
    }
    FlatMirrorModel* mirror_model = dynamic_cast<FlatMirrorModel*>(ui->objectsTableView->model());
    if (mirror_model) {
        mirror_model->setImage(QImage(fileName));
        return;
    }
}

void MainWindow::on_loadImageBack_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    LensModel* lens_model = dynamic_cast<LensModel*>(ui->objectsTableView->model());
    if (lens_model) {
        lens_model->setBackHeightMap(QImage(fileName));
        return;
    }
}

void MainWindow::on_cameraViewBox_stateChanged(int arg1)
{
    if (arg1)
        glWidget->cameraView();
    else
        glWidget->normalView();
    glWidget->updateGL();
}
