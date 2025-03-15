#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include "slavethread.h"
#include "globalclass.h"

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
class SerialPortInfo;

QT_END_NAMESPACE

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortWidget(QWidget *parent = nullptr);

signals:
    int signPeriodMs(int ms);
    void signOpenSerialPort();
    void signCloseSerialPort();

private:
    void initSerialPortUi();
    void connectFun();
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private slots:
    void showPortInfo(int idx);
    void showRequest(const QByteArray &data);
    void pageTurning();

    void serialPortOperation();
    void slotClearReceive();

    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

private:
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

    Console                     *m_console = nullptr;
    SerialPortInfo              *m_serialPortInfo = nullptr;

    QStackedWidget              *m_stackedWidget = nullptr;
    SendSingleMessageWidget     *m_sendSingleMessageWidget = nullptr;
    SendMultipleMessagesWidget  *m_sendMultipleMessagesWidget = nullptr;
    ProtocolTransferWidget      *m_protocolTransferWidget = nullptr;
    HelpWidget                  *m_helpWidget = nullptr;

    QPushButton                 *m_sendSingleMessageBtn;
    QPushButton                 *m_sendMultipleMessagesBtn;
    QPushButton                 *m_protocolTransferBtn;
    QPushButton                 *m_helpBtn;

    QIntValidator               *m_intValidator = nullptr;

};

#endif // SERIALPORTWIDGET_H
