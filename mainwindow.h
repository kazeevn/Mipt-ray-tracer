#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"

#include <QMainWindow>
#include <QItemSelectionModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GLWidget* glWidget;

public slots:
    void savePic();
private slots:
    void on_renderButton_clicked();
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
};

#endif // MAINWINDOW_H
