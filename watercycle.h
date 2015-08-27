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
    QSerialPort* m_serialPort;
    QByteArray m_bytesRead;

signals:

public slots:

private slots:
    QSerialPort *checkPort();
    void openPump();
};

#endif // WATERCYCLE_H
