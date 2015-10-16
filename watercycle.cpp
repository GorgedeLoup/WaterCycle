#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <QThread>
#include "macro.h"
#include "watercycle.h"

Q_LOGGING_CATEGORY(WATER, "WATER")


WaterCycle::WaterCycle(QObject *parent) : QObject(parent)
{
    m_time = new QTime;
    m_timer = new QTimer;

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
    m_queryOrder = 1;

    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
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
        {// Error signal
        qDebug() << "Open pump failed !";}
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
        {// Error signal
        qDebug() << "Close pump failed !";}
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
    {
        emit updateSpeed(speed);
        qDebug() << "Set water flow speed" << speed << "finished ;";
    }
    else
        {// Error signal
        qDebug() << "Set water flow speed failed !";}
}


void WaterCycle::queryStatus()
{
    //qDebug() << "Querying status ...";

    m_serialPort->write(QByteArray::fromHex(S_QUERYSTATUS));
    m_serialPort->waitForReadyRead(500);
    QByteArray back = m_serialPort->read(1);
    if (back != QByteArray::fromHex(S_QUERYSTATUS))
        // Error signal
        qDebug() << back << "Query status failed !";

    // Receive temperature1 information
    QByteArray temp1 = m_serialPort->read(1);
    QString tempStr1 = temp1.toHex().data();
    m_temperature1 = tempStr1.toInt(0, 16);
    emit updateTemp1(m_temperature1);
    // qDebug() << "Temperature1:" << m_temperature1;

    QByteArray temp2 = m_serialPort->read(1);
    QString tempStr2 = temp2.toHex().data();
    m_temperature2 = tempStr2.toInt(0, 16);
    emit updateTemp2(m_temperature2);
    // qDebug() << "Temperature2:" << m_temperature2;

    // Receive water level information
    QByteArray waterLevel = m_serialPort->readAll();
    if (waterLevel == QByteArray::fromHex(R_WATERLOW))
        m_waterLevel = "Low";
    else if (waterLevel == QByteArray::fromHex(R_WATERHIGH))
        m_waterLevel = "High";
    else if (waterLevel == QByteArray::fromHex(R_WATERFULL))
        m_waterLevel = "Full";
    else
    {
        // Error
        qDebug() << waterLevel.toHex();
        m_waterLevel = "Error !";
        qDebug() << "Water level return failed !";
    }

    emit updateWaterLevel(m_waterLevel);
 //   qDebug() << "Water level:" << m_waterLevel;

 //   qDebug() << "Query status finished ;";
}


void WaterCycle::testFunc()
{
    queryStatus();
    qDebug() << "Order:" << m_queryOrder
//             << "Time:" << m_time->currentTime().toString()
             << "Temp1:" << m_temperature1 << "Temp2:" << m_temperature2;
    m_queryOrder += 1;
//    qDebug() << "Start time:" << time.currentTime().toString();
//    for (int i = 0; i < 4000; i++)
//    {
//        queryStatus();
//        qDebug() << i << ":" << m_waterLevel;
//        if (m_waterLevel != "High")
//            //QTest::qWait(10000);
//            QThread::sleep(10);
//        else
//        {
//            qDebug() << "Full to high time:" << time.currentTime().toString();
//            break;
//        }
//    }
//    for (int j = 0; j < 4000; j++)
//    {
//        queryStatus();
//        qDebug() << j << ":" << m_waterLevel;
//        if (m_waterLevel != "Low")
//            //QTest::qWait(10000);
//            QThread::sleep(10);
//        else
//        {
//            qDebug() << "High to low time:" << time.currentTime().toString();
//            break;
//        }
//    }
}


void WaterCycle::setTimer()
{
    m_timer->setSingleShot(false);
    m_timer->setInterval(3000);
    qDebug() << "Start timer." << "   " << "Interval:" << (m_timer->interval())/1000 << "secs.";
    testFunc();
    m_timer->start();
}


void WaterCycle::stopTimer()
{
    qDebug() << "Stop timer.";
    m_timer->stop();
    m_queryOrder = 1;
}


void WaterCycle::timerSlot()
{
    testFunc();
}
