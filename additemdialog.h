#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include "core/objects/stubs/virtual3dobjectstub.h"

namespace Ui {
    class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = 0);
    ~AddItemDialog();

private:
    Ui::AddItemDialog *ui;
    Virtual3DObjectStub* object;

};

#endif // ADDITEMDIALOG_H
