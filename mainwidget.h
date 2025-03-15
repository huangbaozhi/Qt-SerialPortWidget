#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "slavethread.h"

#include <QWidget>
#include <QSerialPort>

QT_BEGIN_NAMESPACE

class Console;
class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QStackedWidget;
class SendSingleMessageWidget;
class SendMultipleMessagesWidget;
class ProtocolTransferWidget;
class HelpWidget;
class QIntValidator;

QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public:
//    struct Settings {
//        QString name;
//        qint32 baudRate;
//        QString stringBaudRate;
//        QSerialPort::DataBits dataBits;
//        QString stringDataBits;
//        QSerialPort::Parity parity;
//        QString stringParity;
//        QSerialPort::StopBits stopBits;
//        QString stringStopBits;
//        QSerialPort::FlowControl flowControl;
//        QString stringFlowControl;
//        bool localEchoEnabled;
//    };

//    Settings settings() const;

signals:
    int signPeriodMs(int ms);

private:
    void initUi();
    void initSerialPortUi();
    void connectFun();
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

    void closeSerialPort();

private slots:
    void startSlave();
    void showRequest(const QByteArray &data);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void activateRunButton();
    void pageTurning();

    void slotClearReceive();

    int slotPeriodMs(int ms);

    void handleError(QSerialPort::SerialPortError error);
    void showPortInfo(int idx);
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

private:
    int          m_transactionCount = 0;
    QLabel      *m_serialPortLabel = nullptr;

    QComboBox   *m_serialPortComboBox = nullptr;
    QComboBox   *m_baudRateComboBox = nullptr;
    QComboBox   *m_stopBitComboBox = nullptr;
    QComboBox   *m_dataBitComboBox = nullptr;
    QComboBox   *m_parityCheckComboBox = nullptr;

    QPushButton *m_serialPortOprationBtn;
    QPushButton *m_saveBtn;
    QPushButton *m_clearBtn;

    QCheckBox   *m_hexShowCheckBox;
    QCheckBox   *m_backgroundColorCheckBox;
    QCheckBox   *m_RTSCheckBox;
    QCheckBox   *m_DTRCheckBox;
    QCheckBox   *m_timeStampCheckBox;

    QLabel      *m_waitRequestLabel = nullptr;
    QSpinBox    *m_waitRequestSpinBox = nullptr;
    QLabel      *m_responseLabel = nullptr;
    QLineEdit   *m_responseLineEdit = nullptr;
    QLabel      *m_trafficLabel = nullptr;
    QLabel      *m_statusLabel = nullptr;
    QPushButton *m_runButton = nullptr;

    SlaveThread m_thread;
    Console     *m_console = nullptr;

    QStackedWidget              *m_stackedWidget = nullptr;
    SendSingleMessageWidget     *m_sendSingleMessageWidget = nullptr;
    SendMultipleMessagesWidget  *m_sendMultipleMessagesWidget = nullptr;
    ProtocolTransferWidget      *m_protocolTransferWidget = nullptr;
    HelpWidget                  *m_helpWidget = nullptr;

    QPushButton                 *m_sendSingleMessageBtn;
    QPushButton                 *m_sendMultipleMessagesBtn;
    QPushButton                 *m_protocolTransferBtn;
    QPushButton                 *m_helpBtn;

    int                         m_periodMs;
    QIntValidator               *m_intValidator = nullptr;
    QSerialPort                 *m_serial = nullptr;


    //Settings                    m_currentSettings;
};
#endif // MAINWIDGET_H
