#include "globalclass.h"

GlobalClass::GlobalClass(QObject *parent) : QObject(parent)
{

}

GlobalClass globalClass;
GlobalClass *G_GlobalClass = &globalClass;
GlobalClass *GlobalClass::instance()
{
    if (G_GlobalClass)
    {
        G_GlobalClass = new GlobalClass;
    }
    return G_GlobalClass;
}

GlobalClass::serialPortDateInfo GlobalClass::serialportData() const
{
    return m_serialPortData;
}

void GlobalClass::setPortName(QString portName)
{
    m_serialPortData.name = portName;
}

void GlobalClass::setPortName(QStringList portNamelist)
{
    m_portNameList = portNamelist;
}

QStringList GlobalClass::getPortName()
{
    return m_portNameList;
}

void GlobalClass::setBaudRate(qint32 baudRate)
{
    m_serialPortData.baudRate = baudRate;
}

void GlobalClass::setStringBaudRate(QString strBaudRate)
{
    m_serialPortData.stringBaudRate = strBaudRate;
}

void GlobalClass::setDataBits(QSerialPort::DataBits dataBits)
{
    m_serialPortData.dataBits = dataBits;
}

void GlobalClass::setStringDataBits(QString strDataBits)
{
    m_serialPortData.stringDataBits = strDataBits;
}

void GlobalClass::setParity(QSerialPort::Parity parity)
{
    m_serialPortData.parity = parity;
}

void GlobalClass::setStringParity(QString strParity)
{
    m_serialPortData.stringParity = strParity;
}

void GlobalClass::setStopBits(QSerialPort::StopBits stopBits)
{
    m_serialPortData.stopBits = stopBits;
}

void GlobalClass::setStringStopBits(QString strStopBits)
{
    m_serialPortData.stringStopBits = strStopBits;
}

void GlobalClass::setConsoleEnabled(bool isEnabled)
{
    m_isConsole = isEnabled;
}

bool GlobalClass::getConsoleEnabled()
{
    return m_isConsole;
}

void GlobalClass::readSerialPortData(QByteArray data)
{
    m_serialPortByteArray = data;
}

QByteArray GlobalClass::getReadSerialPortData()
{
    return m_serialPortByteArray;
}

void GlobalClass::writeSerialPortData(QByteArray data)
{

}
