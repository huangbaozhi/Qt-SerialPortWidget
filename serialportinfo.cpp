#include "serialportinfo.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

SerialPortInfo::SerialPortInfo(QObject *parent)
    : QObject(parent)
    , m_SerialPort(new QSerialPort(this))
{

}

void SerialPortInfo::initSerialPortData()
{
    const auto infos = QSerialPortInfo::availablePorts();

    QStringList list;
    for (const QSerialPortInfo &info : infos)
    {
        list << info.portName();
    }
    G_GlobalClass->setPortName(list);
}

void SerialPortInfo::openSerialPort()
{
    m_SerialPort->setPortName(G_GlobalClass->serialportData().name);
    m_SerialPort->setBaudRate(G_GlobalClass->serialportData().baudRate);
    m_SerialPort->setDataBits(G_GlobalClass->serialportData().dataBits);
    m_SerialPort->setParity(G_GlobalClass->serialportData().parity);
    m_SerialPort->setStopBits(G_GlobalClass->serialportData().stopBits);
    m_SerialPort->setFlowControl(QSerialPort::NoFlowControl);

//    qDebug()<<"[SerialPortInfo::openSerialPort()]= " << G_GlobalClass->serialportData().name;
//    qDebug()<<"[SerialPortInfo::openSerialPort()]= " << G_GlobalClass->serialportData().baudRate;
//    qDebug()<<"[SerialPortInfo::openSerialPort()]= " << G_GlobalClass->serialportData().dataBits;
//    qDebug()<<"[SerialPortInfo::openSerialPort()]= " << G_GlobalClass->serialportData().parity;
//    qDebug()<<"[SerialPortInfo::openSerialPort()]= " << G_GlobalClass->serialportData().stopBits;

    if (m_SerialPort->open(QIODevice::ReadWrite))
    {
        qDebug()<<"打开串口-成功！！！";
        connect(m_SerialPort, &QSerialPort::readyRead, this, &SerialPortInfo::readSerialPortData);
    }
    else
    {
        qDebug()<<"打开串口-失败！！！!!!";
    }
}

void SerialPortInfo::closeSerialPort()
{
    if (m_SerialPort->isOpen())
        m_SerialPort->close();

    qDebug()<<"关闭串口！！！";
}

void SerialPortInfo::readSerialPortData()
{
    QByteArray requestData = m_SerialPort->readAll();
    emit signReadSerialPortData(requestData);
}
