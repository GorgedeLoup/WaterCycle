#include <QObject>
#include <QBitArray>
#include <QByteArray>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <QMessageBox>
#include "watercycle.h"

WaterCycle::WaterCycle(QObject *parent) : QObject(parent)
{
    QSerialPort* serialPort = checkPort();

    if (serialPort)
    {
        m_serialPort = serialPort;
        if (!m_serialPort->isOpen())
            m_serialPort->open(QIODevice::ReadWrite);
        connect(m_serialPort,SIGNAL(error(QSerialPort::SerialPortError)),SLOT(handleError(QSerialPort::SerialPortError)));
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


QSerialPort* WaterCycle::checkPort()
{
    QByteArray bytesSend;
    bytesSend.append("F7");

    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList)
    {
        QSerialPort *serialPort = new QSerialPort(serialPortInfo);
        serialPort->open((QIODevice::ReadWrite));
        serialPort->write(QByteArray::fromHex(bytesSend));
        serialPort->waitForReadyRead(500);
        QByteArray bytesRead = serialPort->readAll();
        if (bytesRead == QByteArray::fromHex("0xF7,0xA5"))
            return serialPort;
        else
            delete serialPort;
    }
}


void WaterCycle::openPump()
{

}
