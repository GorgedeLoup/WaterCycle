#ifndef WATERCYCLE_H
#define WATERCYCLE_H

#include <QObject>
#include <QObject>
#include <QByteArray>
#include <QTime>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(WATER)


class WaterCycle : public QObject
{
    Q_OBJECT
public:
    explicit WaterCycle(QObject *parent = 0);
    ~WaterCycle();

private:
    QSerialPort *m_serialPort;
    QByteArray m_bytesRead;
    QString m_waterLevel;
    qint16 m_waterSpeed;
    qint16 m_temperature1;
    qint16 m_temperature2;
    //const QSerialPortInfo m_serialPortInfo;    // Save the port configuration information

    QTime *m_time;
    QTimer *m_timer;
    qint16 m_queryOrder;

signals:
    void updateSpeed(qint16);
    void updateWaterLevel(QString);
    void updateTemp1(qint16);
    void updateTemp2(qint16);

public slots:
    void openPump();
    void closePump();
    void setSpeed(qint16);
    void queryStatus();
    void testFunc();
    void setTimer();
    void stopTimer();

private slots:
    QSerialPort *checkPort();
    void timerSlot();
};

#endif // WATERCYCLE_H
