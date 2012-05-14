#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"
#include "core/models/model.h"

#include <QMainWindow>
#include <QListView>
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
    SceneModel* scene_model;
    QListView* listView;

public slots:
    void showPic();
    void doRender();
    void savePic();
private slots:
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void removeRow();
};

#endif // MAINWINDOW_H
