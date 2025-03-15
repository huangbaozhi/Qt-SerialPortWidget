#ifndef GLOBALCLASS_H
#define GLOBALCLASS_H

#include <QObject>
#include <QSerialPort>

class GlobalClass : public QObject
{
    Q_OBJECT
public:
    explicit GlobalClass(QObject *parent = nullptr);

    struct serialPortDateInfo {
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

    static GlobalClass *instance();
    serialPortDateInfo serialportData() const;

signals:

public:
    void setPortName(QString portName);
    void setPortName(QStringList portNamelist);
    QStringList getPortName();

    void setBaudRate(qint32 baudRate);
    void setStringBaudRate(QString strBaudRate);

    void setDataBits(QSerialPort::DataBits dataBits);
    void setStringDataBits(QString strDataBits);

    void setParity(QSerialPort::Parity parity);
    void setStringParity(QString strParity);

    void setStopBits(QSerialPort::StopBits stopBits);
    void setStringStopBits(QString strStopBits);

    void setConsoleEnabled(bool isEnabled);
    bool getConsoleEnabled();

    void readSerialPortData(QByteArray data);
    QByteArray getReadSerialPortData();

    void writeSerialPortData(QByteArray data);
private:
    serialPortDateInfo m_serialPortData;

    QStringList m_portNameList;
    QStringList m_baudRate;
    QStringList m_dataBits;
    QStringList m_parity;
    QStringList m_stopBits;

    bool m_isConsole = false;

    QByteArray m_serialPortByteArray;

};
extern GlobalClass *G_GlobalClass;

#endif // GLOBALCLASS_H
