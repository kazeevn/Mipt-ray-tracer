#include<QPixmap>
#include<QFileDialog>

#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "core/objects/stubs/pictureobjectstub.h"
#include "core/objects/stubs/lensobjectstub.h"
#include "core/objects/stubs/thinlensobjectstub.h"
#include "core/objects/stubs/flatmirrorobjectstub.h"
#include "core/models/model.h"
#include "core/geometry/plane3d.h"
#include "core/geometry/vector3d.h"

AddItemDialog::AddItemDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
    m_window=parent;
    ui->setupUi(this);    
    // TODO(kazeevn) make it python-style for item in {...}
    ui->listWidget->addItem("Picture");
    ui->listWidget->addItem("Lens");
    ui->listWidget->addItem("Ideal Lens");
    ui->listWidget->addItem("Flat Mirror");

    current_object = new PictureObjectStub(Point3D(0,0,0),Vector3D(0,0,0), Vector3D(0,0,0), QImage());
    ui->tableView->setModel(new PictureModel(dynamic_cast<PictureObjectStub*>(current_object)));
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    QItemSelectionModel *selectionModel = ui->listWidget->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    ui->listWidget->selectionModel()->select(QItemSelection(
                                                 ui->listWidget->model()->index(0,0)     ,
                                                 ui->listWidget->model()->index(0,0)),
                                             QItemSelectionModel::Select);
    this->setFixedSize(this->width(),this->height());
}

void AddItemDialog::selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    Q_UNUSED(oldSelection);
    Q_UNUSED(newSelection);

    const QString& selected = ui->listWidget->selectedItems()[0]->text();
    if (selected=="Picture") {
        ui->doubleSpinBox_1->setEnabled(false);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->groupBoxSize->setEnabled(false);
        ui->label_1->setText("");
        ui->label_2->setText("");
        ui->groupBoxImage_2->setEnabled(false);
    } else if (selected=="Lens") {
        ui->doubleSpinBox_1->setEnabled(true);
        ui->doubleSpinBox_2->setEnabled(true);
        ui->groupBoxSize->setEnabled(true);
        ui->label_1->setText("Height");
        ui->label_2->setText("Refractive Index");
        ui->groupBoxImage_2->setEnabled(true);
    } else if (selected=="Ideal Lens") {
        ui->doubleSpinBox_1->setEnabled(true);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->groupBoxSize->setEnabled(false);
        ui->label_1->setText("Focus");
        ui->groupBoxImage_2->setEnabled(false);
        ui->label_2->setText("");
    } else if (selected=="Flat Mirror") {
        ui->doubleSpinBox_1->setEnabled(true);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->groupBoxSize->setEnabled(false);
        ui->label_1->setText("Reflection Index");
        ui->label_2->setText("");
        ui->groupBoxImage_2->setEnabled(false);
    }
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}

void AddItemDialog::on_pushButtonOK_clicked()
{
    if (ui->listWidget->selectedItems().length()==0) return;
    const QString& selected = ui->listWidget->selectedItems()[0]->text();
    if (selected=="Picture") {
        if (!current_object->isValid()) return;
        Scene::Instance().addStubObject(ui->editName->text(), current_object);
    } else if (selected=="Lens") {
        LensObjectStub* lens_object = new LensObjectStub(current_object->point(), current_object->v1(), current_object->v2(),
                                         m_image,m_image_rear,QSize(ui->spinBox->value(),ui->spinBox_2->value()),
                                         ui->spinBox->value(),ui->spinBox_2->value());
        if (!current_object->isValid())
        {
            delete lens_object;
            return;
        }
        Scene::Instance().addStubObject(ui->editName->text(), lens_object);
    } else if (selected=="Ideal Lens") {
        ThinLensObjectStub* thin_lens_object = new ThinLensObjectStub(current_object->point(), current_object->v1(), current_object->v2(),
                                                                      m_image,ui->doubleSpinBox_1->value());
        if (!thin_lens_object->isValid()) {
            delete thin_lens_object;
            return;
        }
        Scene::Instance().addStubObject(ui->editName->text(), thin_lens_object);
    } else if (selected=="Flat Mirror") {
        FlatMirrorObjectStub* flat_mirror_object = new FlatMirrorObjectStub(current_object->point(), current_object->v1(), current_object->v2(),
                                                                      m_image,ui->doubleSpinBox_1->value());
        if (!flat_mirror_object->isValid()) {
            delete flat_mirror_object;
            return;
        }
        Scene::Instance().addStubObject(ui->editName->text(), flat_mirror_object);
    }
    m_window->refresh();
    this->close();
}

void AddItemDialog::on_loadImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    m_image.load(fileName);
    out_image.addPixmap(QPixmap::fromImage(m_image));
    ui->graphicsView->setScene(&out_image);
    current_object->setImage(m_image);
}

void AddItemDialog::on_loadImage2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    m_image_rear.load(fileName);
    out_image_rear.addPixmap(QPixmap::fromImage(m_image_rear));
    ui->graphicsView_2->setScene(&out_image_rear);
}

void AddItemDialog::on_Cancel_clicked()
{
    this->close();
}
