#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include "macro.h"
#include "watercycle.h"

WaterCycle::WaterCycle(QObject *parent) : QObject(parent)
{
    QSerialPort *serialPort = checkPort();

    if (serialPort)
    {
        m_serialPort = serialPort;
        if (!m_serialPort->isOpen())
            m_serialPort->open(QIODevice::ReadWrite);
        //connect(m_serialPort,SIGNAL(error(QSerialPort::SerialPortError)),SLOT(handleError(QSerialPort::SerialPortError)));
    }
    else
    {
        qDebug() << "Open serial port failed !";
    }
    m_bytesRead = "";
}

WaterCycle::~WaterCycle()
{
}


QSerialPort *WaterCycle::checkPort()
{
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
    {
        QSerialPort *serialPort = new QSerialPort(serialPortInfo);
        serialPort->open((QIODevice::ReadWrite));
        serialPort->write(QByteArray::fromHex(S_CHECK_PORT));
        serialPort->waitForReadyRead(500);
        QByteArray bytesRead = serialPort->readAll();
        if (bytesRead == QByteArray::fromHex(R_DEVICE_CODE))
        {
            qDebug() << endl << "Port number:" << serialPort << ";";
            m_serialPort = serialPort;
            return serialPort;
        }
        else
            delete serialPort;
    }
    return NULL;
}


void WaterCycle::openPump()
{
    qDebug() << "Opening pump ...";

    m_serialPort->write(QByteArray::fromHex(S_OPEN_PUMP));
    m_serialPort->waitForReadyRead(500);
    QByteArray bytesRead = m_serialPort->readAll();

    if (bytesRead == QByteArray::fromHex(R_FINISH))
        qDebug() << "Open pump finished ;";
    else
        // Error signal
        qDebug() << "Open pump failed !";
}


void WaterCycle::closePump()
{
    qDebug() << "Closing pump ...";

    m_serialPort->write(QByteArray::fromHex(S_CLOSE_PUMP));
    m_serialPort->waitForReadyRead(500);
    QByteArray bytesRead = m_serialPort->readAll();

    if (bytesRead == QByteArray::fromHex(R_FINISH))
        qDebug() << "Close pump finished ;";
    else
        // Error signal
        qDebug() << "Close pump failed !";
}


void WaterCycle::setSpeed(qint16 speed)
{
    QByteArray bytesSend;
    QString speedStr = QString::number(speed, 16);
    bytesSend.append(speedStr);

    qDebug() << "Set water flow speed:" << speed << "...";

    m_serialPort->write(QByteArray::fromHex(bytesSend));
    m_serialPort->waitForReadyRead(500);
    QByteArray bytesRead = m_serialPort->readAll();

    if (bytesRead == QByteArray::fromHex(R_FINISH))
        qDebug() << "Set water flow speed" << speed << "finished ;";
    else
        // Error signal
        qDebug() << "Set water flow speed failed !";
}


void WaterCycle::queryStatus()
{
    qDebug() << "Querying status ...";

    m_serialPort->write(QByteArray::fromHex(S_QUERYSTATUS));
    m_serialPort->waitForReadyRead(500);
    QByteArray back = m_serialPort->read(1);
    if (back != QByteArray::fromHex(S_QUERYSTATUS))
        // Error signal
        qDebug() << "Query status failed !";

    QByteArray temp1 = m_serialPort->read(1);
    temp1 = m_temperature1;

    QByteArray temp2 = m_serialPort->read(1);
    temp2 = m_temperature2;

    QByteArray waterLevel = m_serialPort->readAll();
    waterLevel = m_waterLevel;

    qDebug() << "Query status finished ;";
}
