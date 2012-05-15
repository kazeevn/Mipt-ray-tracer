#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <cmath>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBtnClicked()
{
    qDebug() << "CLICK";
    if (!image) delete image;
    image = new QImage(QSize(ui->widthBox->value(), ui->picHeightBox->value()), QImage::Format_RGB32);
    image->fill(Qt::white);
    qDebug() << image->size();

    double r = ui->radiusBox->value();
    double h = ui->lensHeightBox->value();
    int x0 = -image->size().width()/2;
    int y0 = -image->size().height()/2;
    for (int i = 0; i < image->size().width(); i++) {
        for (int j = 0; j < image->size().height(); j++) {
            double dx = double(i+x0) / image->size().width() * ui->lensHorSizeBox->value();
            double dy = double(j+y0) / image->size().height() * ui->lensVertSizeBox->value();
            double ch = sqrt(qMax(r*r-(dx*dx+dy*dy), (r-h)*(r-h))) - (r-h);
            QColor c = QColor::fromRgbF(1-ch/h, 1-ch/h, 1-ch/h);
          //  qDebug() << i << j << dx << dy << ch/h;
            image->setPixel(i, j, c.rgb());
        }
        //qDebug() << "---";
    }
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*image));
    ui->graphicsView->setScene(scene);
}


void MainWindow::saveClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
        return;
    image->save(fileName);
}
