#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QPixmap>
#include <QPainter>
#include<QGraphicsView>
#include<QGraphicsScene>
#include <QTransform>
#include <QDebug>
#include <QGraphicsItem>
#include <QMatrix>


//
int couner = 0;
bool on_off = true;
QString fileName;
int zoomPort=0;
int currAngle =0;
int xx;
int yy;
int offsetAngle=45;
double sx, sy;
int routate=0;
//


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    select=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_SaveButton_clicked()
{
    MainWindow::Save();
}


void MainWindow::on_UndoButton_clicked()
{
    MainWindow::Undo();
}

void MainWindow::on_RedoButton_clicked()
{
     MainWindow::Redo();
}

void MainWindow::on_CropButton_clicked()
{
     MainWindow::Crop();
}

void MainWindow::on_SelectButton_clicked()
{
     MainWindow::SelectImage();
}

void MainWindow::on_ResetButton_clicked()
{
     MainWindow::Reset();
}


void MainWindow::on_OpenButton_clicked()
{
   MainWindow::Open();
}


//open image
void MainWindow::on_actionOpen_triggered()
{
    MainWindow::Open();
}

//save image
void MainWindow::on_actionSave_triggered()
{
    MainWindow::Save();
}

//Crop
void MainWindow::on_actionCrop_triggered()
{
    MainWindow::Crop();
}


//select
void MainWindow::on_actionSelect_triggered()
{

    MainWindow::SelectImage();
}

//Reset
void MainWindow::on_actionReset_triggered()
{

   MainWindow::Reset();
}

//undo
void MainWindow::on_actionUndo_triggered()
{
    MainWindow::Undo();
}

//redo
void MainWindow::on_actionRedo_triggered()
{
    MainWindow::Redo();
}



void MainWindow::mousePressEvent(QMouseEvent* event){
    if(select==1){
        origin = event->pos();
        if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();

     }

}

void MainWindow::mouseMoveEvent(QMouseEvent* event){
        if(select==1){
           rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
        }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(select==1){
    distination=event->globalPos();
    a = mapToGlobal(origin);
    a = ui->imageLabel->mapFromGlobal(a);
    distination = ui->imageLabel->mapFromGlobal(distination);
    QPixmap OriginalPix(*ui->imageLabel->pixmap());
    sx = ui->imageLabel->rect().width();
    sy = ui->imageLabel->rect().height();
    sx = OriginalPix.width() / sx;
    sy = OriginalPix.height() / sy;
    a.setX(int(a.x() * sx));
    distination.setX(int(distination.x() * sx));
    a.setY( int(a.y() * sy));
    distination.setY( int(distination.y() * sy));
    if(a.x()<distination.x()&&a.y()<distination.y()){
    }
    else if(a.x()<distination.x()&&a.y()>distination.y()){
    int temp=a.y();
    a.setY(distination.y());
    distination.setY(temp);
    }
    else if(a.x()>distination.x()&&a.y()<distination.y()){
    int temp=a.x();
    a.setX(distination.x());
    distination.setX(temp);
    }
    else{
    int temp=a.x();
    a.setX(distination.x());
    distination.setX(temp);
    temp=a.y();
    a.setY(distination.y());
    distination.setY(temp);
    }
}
}



//method implementation

void MainWindow::Save() {

     if(openImage==1){
    if(select==1){
       rubberBand->hide();
    }
    QString imagePath = QFileDialog::getSaveFileName(this, tr("Save File"),"C:/",tr("Images (*.png *.bmp *.jpg)"));
    image.save(imagePath);
    select=0;
}}


void MainWindow::Crop() {
    if(openImage==1){
       if(select==1){
       rubberBand->hide();
       QRect rect(a, distination);
       image = image.copy(rect);
       ui->imageLabel->setPixmap(QPixmap::fromImage(image));
       undoStack->push(image);
       }
        select=0;
   }
}

void MainWindow::SelectImage() {
    if(openImage==1){
    select=1;
}}

void MainWindow::Reset() {
    if(openImage==1){
        if(select==1){
           rubberBand->hide();
        }
        while(undoStack->size()>1){
            (undoStack->pop());
        }
        image=undoStack->pop();
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
        ui->imageLabel->setScaledContents( false );
        ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

        undoStack = new QStack <QImage>;
        redoStack = new QStack <QImage>;
        undoStack->push(image);
        select=0;
}}

void MainWindow::Undo() {
    if(openImage==1){
    if(select==1){
       rubberBand->hide();
    }
    if(undoStack->size() > 1){
    redoStack->push(undoStack->pop());
    image=undoStack->top();
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setScaledContents( false );
    ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    }
    select=0;
}}


void MainWindow::Redo() {
    if(openImage==1){
    if(select==1){
       rubberBand->hide();
    }
    if(redoStack->size() > 0){
        image=redoStack->top();
        undoStack->push(redoStack->pop());
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
        ui->imageLabel->setScaledContents( false );
        ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

     }
    select=0;}
}

void MainWindow::Open() {

    if(select==1){
       rubberBand->hide();
    }
   undoStack = new QStack <QImage>;
   redoStack = new QStack <QImage>;
   QFileDialog dialog(this);
   dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
   dialog.setViewMode(QFileDialog::Detail);
   fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"C:/",tr("Images (*.png *.bmp *.jpg)"));
   image.load(fileName);
   ui->imageLabel->setPixmap(QPixmap::fromImage(image));
   ui->imageLabel->setScaledContents( false );
   ui->imageLabel->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

   undoStack->push(image);
   select=0;
   openImage=1;
}



void MainWindow::ZoomIn()
{if(openImage==1){
  if(select==1){
    rubberBand->hide();
    QRect rect(a, distination);
    image = image.copy(rect);
    int w=(int)image.width()*2;
    int h=(int)image.height()*2;
    image = image.scaled(w, h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
   // undoStack->push(image);
   }else{

      int w=(int)image.width()*1.25;
      int h=(int)image.height()*1.25;
      image = image.scaled(w, h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      ui->imageLabel->setPixmap(QPixmap::fromImage(image));

      undoStack->push(image);
   }
  select=0;}

}

void MainWindow::ZoomOut()
{if(openImage==1){
    if(select==1){
        rubberBand->hide();
        QRect rect(a, distination);
        image = image.copy(rect);
        int w=(int)image.width()*0.9;
        int h=(int)image.height()*0.9;
        image = image.scaled(w, h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
       // undoStack->push(image);
    }
    int w=(int)image.width()*0.9;
    int h=(int)image.height()*0.9;
    image = image.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    undoStack->push(image);
    select=0;

}
}

void MainWindow::rotate(int angle)
{if(openImage==1){
    if(select==1){
        rubberBand->hide();
    }
    QPixmap rotatedPixmap((QPixmap::fromImage(image) ).size());
    rotatedPixmap.fill(QColor::fromRgb(0, 0, 0, 0)); //the new pixmap must be transparent.
    QPainter* p = new QPainter(&rotatedPixmap);
    QSize size = (QPixmap::fromImage(image) ).size();
    p->translate(size.width()/2,size.height()/2);
    currAngle=angle+currAngle;
    p->rotate(currAngle);

    p->translate(-size.width()/2,-size.height()/2);
    p->drawPixmap(0,0, (QPixmap::fromImage(image) ));

    p->end();
    delete p;

    ui->imageLabel->setPixmap(rotatedPixmap);
        image = rotatedPixmap.toImage();




    undoStack->push(image);
    select=0;}
}

void MainWindow::on_ZoominButton_clicked()
{
    ZoomIn();
}

void MainWindow::on_toolButton_3_clicked()
{
    ZoomOut();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    rotate(value);
}

void MainWindow::on_actionRotate_90_triggered()
{if(openImage==1){
    if(select==1){
        rubberBand->hide();
    }
      QPixmap pixmap(QPixmap::fromImage(image));
      QMatrix rm;
      rm.rotate(90);
      pixmap = pixmap.transformed(rm);
      ui->imageLabel->setPixmap(pixmap);
      image = pixmap.toImage();
      undoStack->push(image);
      select=0;
}}
