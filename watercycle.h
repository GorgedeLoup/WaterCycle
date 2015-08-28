#ifndef WATERCYCLE_H
#define WATERCYCLE_H

#include <QObject>
#include <QObject>
#include <QByteArray>
#include <QtSerialPort/QSerialPort>

class WaterCycle : public QObject
{
    Q_OBJECT
public:
    explicit WaterCycle(QObject *parent = 0);
    ~WaterCycle();

private:
    QSerialPort *m_serialPort;
    QByteArray m_bytesRead;
    qint16 m_waterLevel;
    qint16 m_waterSpeed;
    qint16 m_temperature1;
    qint16 m_temperature2;
    //const QSerialPortInfo m_serialPortInfo;    // Save the port configuration information

signals:

public slots:
    void openPump();
    void closePump();
    void setSpeed(qint16);
    void queryStatus();

private slots:
    QSerialPort *checkPort();
};

#endif // WATERCYCLE_H
