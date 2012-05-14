#include<QPixmap>
#include<QFileDialog>

#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "core/objects/stubs/pictureobjectstub.h"
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
    ui->listWidget->addItem("Lense");
    ui->listWidget->addItem("Ideal Lense");
    ui->listWidget->addItem("Flat Mirror");

    current_object = new PictureObjectStub(Point3D(0,0,0),Vector3D(0,0,0), Vector3D(0,0,0), QImage());
    ui->tableView->setModel(new PictureModel(dynamic_cast<PictureObjectStub*>(current_object)));
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    QItemSelectionModel *selectionModel = ui->listWidget->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
}

void AddItemDialog::selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    Q_UNUSED(oldSelection);
    Q_UNUSED(newSelection);

    const QString& selected = ui->listWidget->selectedItems()[0]->text();
    if (selected=="Picture") {
        ui->doubleSpinBox_1->setEnabled(false);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->label_1->setText("");
        ui->label_2->setText("");
    } else if (selected=="Lense") {
        ui->doubleSpinBox_1->setEnabled(true);
        ui->doubleSpinBox_2->setEnabled(true);
        ui->label_1->setText("Height");
        ui->label_2->setText("Refractive Index");
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
        m_window->refresh();
        this->close();
    }
}

void AddItemDialog::on_loadImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Save Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    m_image.load(fileName);
    out_image.addPixmap(QPixmap::fromImage(m_image));
    ui->graphicsView->setScene(&out_image);
    current_object->setImage(m_image);
}
