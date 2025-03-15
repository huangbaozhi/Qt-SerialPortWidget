#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H

//#include "globalclass.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>

class SlaveThread : public QThread
{
    Q_OBJECT
public:
    explicit SlaveThread(QObject *parent = nullptr);
    ~SlaveThread();

    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings settings() const;

signals:
    void request(const QByteArray &data);
    //void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

public:
    void startSlave(const QString &portName, int waitTimeout, const QString &response);

private:
    void run() override;

    void openSerialPort();
    void closeSerialPort();


private:
    QString m_portName;
    QString m_response;
    int     m_waitTimeout = 0;
    QMutex  m_mutex;
    bool    m_quit = false;

    QSerialPort *m_serial = nullptr;

    Settings m_currentSettings;
};

#endif // SLAVETHREAD_H
