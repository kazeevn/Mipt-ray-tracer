#include "additemdialog.h"
#include "ui_additemdialog.h"

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
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}
