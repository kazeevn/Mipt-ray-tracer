#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QItemSelectionModel>
#include <QImage>
#include <QGraphicsScene>

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
    Virtual3DObjectStub* current_object;
    QImage m_image;
    QGraphicsScene out_image;

private slots:
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void on_pushButtonOK_clicked();
    void on_loadImage_clicked();
};

#endif // ADDITEMDIALOG_H
