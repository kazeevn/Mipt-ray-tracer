#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "core/objects/stubs/pictureobjectstub.h"
#include "core/models/model.h"
#include "core/geometry/plane3d.h"
#include "core/geometry/vector3d.h"

AddItemDialog::AddItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
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
