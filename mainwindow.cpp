#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "prevqwidget.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cam = new TestCam;
}

MainWindow::~MainWindow()
{
    delete cam;
    delete ui;
}


void MainWindow::on_pushButton_Start_clicked()
{
    ui->statusbar->showMessage("Started");
    connect(cam, &TestCam::imageReady, ui->widget, &PrevQWidget::slotGetNewImage, Qt::QueuedConnection);
    connect(cam, &TestCam::imageReady, ui->openGLWidget, &PrevGLWidget::slotGetNewImage, Qt::QueuedConnection);
    cam->startCapture();
    ui->pushButton_Start->setDisabled(true);
    ui->pushButton_Stop->setEnabled(true);
}


void MainWindow::on_pushButton_Stop_clicked()
{
    ui->statusbar->showMessage("Stopped");
    cam->stopCapture();
    disconnect(cam, &TestCam::imageReady, ui->widget, &PrevQWidget::slotGetNewImage);
    disconnect(cam, &TestCam::imageReady, ui->openGLWidget, &PrevGLWidget::slotGetNewImage);
    ui->pushButton_Stop->setDisabled(true);
    ui->pushButton_Start->setEnabled(true);
}

