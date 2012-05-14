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
    void refresh() {scene_model->refresh();}

private:
    Ui::MainWindow *ui;
    GLWidget* glWidget;
    SceneModel* scene_model;
public slots:
    void showPic();
    void doRender();
    void savePic();

    void saveScene();
    void loadScene();
private slots:
    void selectionChangedSlot(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void removeRow();
    void addItem();
};

#endif // MAINWINDOW_H
