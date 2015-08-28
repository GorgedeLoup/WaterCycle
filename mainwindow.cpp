#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "watercycle.h"


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


void MainWindow::on_u_openPumpButton_clicked()
{
    water.openPump();
}


void MainWindow::on_u_closePumpButton_clicked()
{
    water.closePump();
}


void MainWindow::on_u_speedButton_clicked()
{
    water.setSpeed(ui->u_speedLineEdit->text().toInt());
}


void MainWindow::on_u_queryButton_clicked()
{
    water.queryStatus();
}
