#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "watercycle.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&water, SIGNAL(updateSpeed(qint16)), this, SLOT(setSpeedLabel(qint16)));
    connect(&water, SIGNAL(updateWaterLevel(QString)), this, SLOT(setWaterLevelLabel(QString)));
    connect(&water, SIGNAL(updateTemp1(qint16)), this, SLOT(setTemp1Label(qint16)));
    connect(&water, SIGNAL(updateTemp2(qint16)), this, SLOT(setTemp2Label(qint16)));
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

void MainWindow::setSpeedLabel(qint16 speed)
{
    ui->u_speedValue->setText(QString::number(speed));
}

void MainWindow::setWaterLevelLabel(QString waterLevel)
{
    ui->u_waterValue->setText(waterLevel);
}

void MainWindow::setTemp1Label(qint16 temp1)
{
    ui->u_temperatureValue1->setText(QString::number(temp1, 10));
}

void MainWindow::setTemp2Label(qint16 temp2)
{
    ui->u_temperatureValue2->setText(QString::number(temp2, 10));
}

void MainWindow::on_u_testButton_clicked()
{
    if(ui->u_testButton->isChecked())
        water.setTimer();
    else
        water.stopTimer();
}
