#include "slavethread.h"
//#include "globalclass.h"

#include <QSerialPort>
#include <QTime>
#include <QDebug>
#include <QMessageBox>

SlaveThread::SlaveThread(QObject *parent)
    : QThread(parent)
    //, m_serial(new QSerialPort(this))
{

}

SlaveThread::~SlaveThread()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    wait();
}

SlaveThread::Settings SlaveThread::settings() const
{
    return m_currentSettings;
}

void SlaveThread::startSlave(const QString &portName, int waitTimeout, const QString &response)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_response = response;

    if (!isRunning())
        start();
}

void SlaveThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();

    QString currentPortName;
    if (currentPortName != m_portName)
    {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRespone = m_response;
    m_mutex.unlock();

    QSerialPort serial;
    serial.setBaudRate(QSerialPort::Baud115200);          // 设置波特率
    serial.setDataBits(QSerialPort::Data8);             // 数据位8
    serial.setParity(QSerialPort::NoParity);            // 无校验
    serial.setStopBits(QSerialPort::OneStop);           // 1位停止位
    serial.setFlowControl(QSerialPort::NoFlowControl);  // 无流控制

    //openSerialPort();

    while (!m_quit)
    {
        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite))
            {
                emit error(tr("Can't open %1, error code %2").arg(m_portName).arg(m_serial->error()));
                return;
            }
        }

        if (serial.waitForReadyRead(currentWaitTimeout))
        {
            // read request
            QByteArray requestData = serial.readAll();
            qDebug() <<"[]000== " << requestData;

            while (serial.waitForReadyRead(10))
            {
                requestData += serial.readAll();
            }

            // write response
            const QByteArray responseData = currentRespone.toUtf8();
            serial.write(responseData);
            qDebug() <<"[]111== " << responseData;

            if (serial.waitForBytesWritten(m_waitTimeout))
            {
//                const QString request = QString::fromUtf8(requestData);
//                qDebug()<<"[]= " << request;
//                emit this->request(request);
                emit this->request(requestData);
            }
            else
            {
                emit timeout(tr("Wait write response timeout %1").arg(QTime::currentTime().toString()));
            }
        }
        else
        {
            emit timeout(tr("Wait read request timeout %1").arg(QTime::currentTime().toString()));
        }

        m_mutex.lock();

        if (currentPortName != m_portName)
        {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        }
        else
        {
            currentPortNameChanged = false;
        }

        currentWaitTimeout = m_waitTimeout;
        currentRespone = m_response;
        m_mutex.unlock();
    }
}

void SlaveThread::openSerialPort()
{
}

void SlaveThread::closeSerialPort()
{

}
