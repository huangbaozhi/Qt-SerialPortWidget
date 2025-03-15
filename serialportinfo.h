#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H

#include "globalclass.h"
#include <QObject>

class QSerialPort;

class SerialPortInfo : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortInfo(QObject *parent = nullptr);

signals:
    void signReadSerialPortData(const QByteArray &data);

public slots:
    void initSerialPortData();
    void openSerialPort();
    void closeSerialPort();

private:
    //void openSerialPort();
    //void closeSerialPort();

    void readSerialPortData();

private:
    QSerialPort *m_SerialPort;

};

#endif // SERIALPORTINFO_H
