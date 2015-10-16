#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "watercycle.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_u_openPumpButton_clicked();
    void on_u_closePumpButton_clicked();
    void on_u_speedButton_clicked();
    void on_u_queryButton_clicked();

    void setSpeedLabel(qint16 speed);
    void setWaterLevelLabel(QString waterLevel);
    void setTemp1Label(qint16 temp1);
    void setTemp2Label(qint16 temp2);

    void on_u_testButton_clicked();

private:
    Ui::MainWindow *ui;

    WaterCycle water;
};

#endif // MAINWINDOW_H
