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
    ui->setupUi(this);

    RenderedImage::init(QSize(0, 0));

    connect(&Scene::Instance(), SIGNAL(renderingFinished()), this, SLOT(showPic()));

    glWidget = new GLWidget;
    QTableView* tableView = new QTableView;
    listView = new QListView;
    QTableView* tableViewCamera = new QTableView;
    QPushButton* deleteItem = new QPushButton;
    QPushButton* addItem = new QPushButton;
    QCheckBox* viewFromCamera = new QCheckBox;
    QGridLayout* editor_layout = new QGridLayout;
    QWidget* buttons = new QWidget;
    QHBoxLayout* buttons_layout = new QHBoxLayout;

    ui->tabWidget->widget(0)->setLayout(editor_layout);
    ui->tabWidget->activateWindow();
    editor_layout->addWidget(tableView,0,0);
    editor_layout->addWidget(listView,1,0);
    editor_layout->addWidget(buttons,2,0);
    editor_layout->addWidget(tableViewCamera,3,0);
    editor_layout->addWidget(glWidget,0,1,4,1);
    editor_layout->setColumnStretch(1,1);

    buttons->setLayout(buttons_layout);
    buttons_layout->addWidget(deleteItem);
    buttons_layout->addWidget(addItem);
    buttons_layout->addWidget(viewFromCamera);

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

    scene_model=new SceneModel;
    CameraModel *camera_model = new CameraModel(glWidget);
    Virtual3DObjectDelegate *pic_delegate = new Virtual3DObjectDelegate(tableView, glWidget);
    listView->setModel(scene_model);
    listView->setItemDelegate(pic_delegate);
    QItemSelectionModel *selectionModel = listView->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    connect(deleteItem, SIGNAL(clicked()),this,SLOT(removeRow()));
    connect(addItem,SIGNAL(clicked()),this,SLOT(addItem()));

    tableViewCamera->setModel(camera_model);
    tableViewCamera->resizeRowsToContents();
    tableViewCamera->resizeColumnsToContents();
    deleteItem->setText("Delete");
    addItem->setText("Add");
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
    foreach(const QModelIndex& index, listView->selectionModel()->selectedRows())
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
    Scene::Instance().saveStubsToFile("stubs.scene");
    Scene::Instance().loadStubsFromFile("stubs.scene");

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
