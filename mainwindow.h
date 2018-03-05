#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QRubberBand>
#include <QtCore/QStack>
#include <QtCore/QVariant>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QImage image;
    QStack<QImage> * undoStack;
    QStack<QImage> * redoStack;
    QString fileName;
    QPixmap imageP;
    double scaleFactor=1;
    int select;
    int openImage=0;
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionCrop_triggered();
    void on_actionSelect_triggered();
    void on_actionReset_triggered();
    void on_actionUndo_triggered();
    void on_actionSave_triggered();
    void on_actionRedo_triggered();

    void on_OpenButton_clicked();

    void on_SaveButton_clicked();

    void on_UndoButton_clicked();

    void on_RedoButton_clicked();

    void on_CropButton_clicked();

    void on_SelectButton_clicked();

    void on_ResetButton_clicked();

    void on_ZoominButton_clicked();

    void on_toolButton_3_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_actionRotate_90_triggered();

private:
    Ui::MainWindow *ui;
    QRubberBand* rubberBand{rubberBand = NULL};
    QPoint origin;
    QPoint distination;
    QPoint a;
    void Open();
    void Save();
    void Crop();
    void SelectImage();
    void Undo();
    void Redo();
    void Reset();
    void rotate(int angle);
    void ZoomIn();
    void ZoomOut();


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
