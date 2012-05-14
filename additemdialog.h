#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QItemSelectionModel>
#include <QImage>
#include <QGraphicsScene>

#include "core/objects/stubs/pictureobjectstub.h"
#include "mainwindow.h"

namespace Ui {
    class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(MainWindow *parent = 0);
    ~AddItemDialog();

private:
    Ui::AddItemDialog *ui;
    PictureObjectStub* current_object;
    QImage m_image;
    QImage m_image_rear;
    QGraphicsScene out_image;
    QGraphicsScene out_image_rear;
    MainWindow* m_window;

private slots:
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void on_pushButtonOK_clicked();
    void on_loadImage_clicked();
    void on_loadImage2_clicked();
};

#endif // ADDITEMDIALOG_H
