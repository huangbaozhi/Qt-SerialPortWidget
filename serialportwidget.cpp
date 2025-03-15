#include "serialportwidget.h"
#include "console.h"
#include "sendsinglemessagewidget.h"
#include "sendmultiplemessageswidget.h"
#include "protocoltransferwidget.h"
#include "helpwidget.h"
#include "serialportinfo.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QStackedWidget>
#include <QMessageBox>
#include <QThread>
#include <QDebug>

SerialPortWidget::SerialPortWidget(QWidget *parent)
    : QWidget(parent)
    , m_intValidator(new QIntValidator(0, 4000000, this))
{
    QThread *pSerialPortThread = new QThread;
    m_serialPortInfo = new SerialPortInfo;
    m_serialPortInfo->moveToThread(pSerialPortThread);
    pSerialPortThread->start();
    connect(pSerialPortThread, &QThread::started, m_serialPortInfo,
            &SerialPortInfo::initSerialPortData);

    // 初始化
    initSerialPortUi();

    // 信号槽连接
    connectFun();

    // 波特率、停止位、数据位、奇偶校验
    fillPortsParameters();

    // 串口选择
    fillPortsInfo();

    // 更新当前串口选择
    updateSettings();
}

void SerialPortWidget::initSerialPortUi()
{
    setWindowTitle(tr("仿串口界面"));
    setWindowIcon(QIcon(":/images/logo/logo_64.ico"));

    QVBoxLayout *pMainLyt = new QVBoxLayout(this);
    pMainLyt->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *pTopLyt = new QHBoxLayout;
    pTopLyt->setContentsMargins(0, 0, 0, 0);
    pMainLyt->addLayout(pTopLyt);

    QVBoxLayout *pConsoleLyt = new QVBoxLayout;
    m_console = new Console;
    m_console->resize(523, 321);
    pConsoleLyt->addWidget(m_console);
    pTopLyt->addLayout(pConsoleLyt);

    // right widget
    QWidget *pRightWgt = new QWidget(this);
    pTopLyt->addWidget(pRightWgt);

    QVBoxLayout *pVrightLyt = new QVBoxLayout(pRightWgt);
    pVrightLyt->setContentsMargins(0, 0, 0, 0);

    QGridLayout *pRightLyt = new QGridLayout;
    pRightLyt->setContentsMargins(0, 0, 5, 0);
    pVrightLyt->addLayout(pRightLyt);
    pVrightLyt->addStretch();

    // 串口选择
    QLabel *pSerialPortSeletionLbl = new QLabel(this);
    pSerialPortSeletionLbl->setText("串口选择");
    m_serialPortComboBox = new QComboBox;
    pRightLyt->addWidget(pSerialPortSeletionLbl, 0, 0, 1, 1);
    pRightLyt->addWidget(m_serialPortComboBox, 1, 0, 1, 2);

//    const auto infos = QSerialPortInfo::availablePorts();
//    for (const QSerialPortInfo &info : infos)
//    {
//        m_serialPortComboBox->addItem(info.portName());
//    }

    // 波特率
    QLabel *pBaudRateLbl = new QLabel(this);
    pBaudRateLbl->setText("波特率");
    m_baudRateComboBox = new QComboBox;
    pRightLyt->addWidget(pBaudRateLbl, 2, 0);
    pRightLyt->addWidget(m_baudRateComboBox, 2, 1);

    // 停止位
    QLabel *pStopBitLbl = new QLabel(this);
    pStopBitLbl->setText("停止位");
    m_stopBitComboBox = new QComboBox;
    pRightLyt ->addWidget(pStopBitLbl, 3, 0);
    pRightLyt->addWidget(m_stopBitComboBox, 3, 1);

    // 数据位
    QLabel *pDataBitLbl = new QLabel(this);
    pDataBitLbl->setText("数据位");
    m_dataBitComboBox = new QComboBox;
    pRightLyt ->addWidget(pDataBitLbl, 4, 0);
    pRightLyt->addWidget(m_dataBitComboBox, 4, 1);
    m_dataBitComboBox->setCurrentIndex(3);

    // 奇偶校验
    QLabel *pParityCheckLbl = new QLabel(this);
    pParityCheckLbl->setText("奇偶校验");
    m_parityCheckComboBox = new QComboBox;
    pRightLyt ->addWidget(pParityCheckLbl, 5, 0);
    pRightLyt->addWidget(m_parityCheckComboBox, 5, 1);

    // 串口操作
    QLabel *pSerialPortOprationLbl = new QLabel(this);
    pSerialPortOprationLbl->setText("串口操作");
    m_serialPortOprationBtn = new QPushButton;
    m_serialPortOprationBtn->setText("打开串口");
    pRightLyt->addWidget(pSerialPortOprationLbl, 6, 0);
    pRightLyt->addWidget(m_serialPortOprationBtn, 6, 1);

    // 保存、清除
    m_saveBtn = new QPushButton;
    m_saveBtn->setText("保存窗口");
    m_clearBtn = new QPushButton;
     m_clearBtn->setText("清除接收");
    pRightLyt->addWidget(m_saveBtn, 7, 0);
    pRightLyt->addWidget(m_clearBtn, 7, 1);

    //
    m_hexShowCheckBox = new QCheckBox;
    m_hexShowCheckBox->setText("16进制显示");

    m_backgroundColorCheckBox = new QCheckBox;
    m_backgroundColorCheckBox->setText("白底黑字");

    m_RTSCheckBox = new QCheckBox;
    m_RTSCheckBox->setText("RTS");

    m_DTRCheckBox = new QCheckBox;
    m_DTRCheckBox->setText("DTR");

    m_timeStampCheckBox = new QCheckBox;
    m_timeStampCheckBox->setText("时间戳(以换行回车断帧)");
    pRightLyt->addWidget(m_hexShowCheckBox, 8, 0);
    pRightLyt->addWidget(m_backgroundColorCheckBox, 8, 1);
    pRightLyt->addWidget(m_RTSCheckBox, 9, 0);
    pRightLyt->addWidget(m_DTRCheckBox, 9, 1);
    pRightLyt->addWidget(m_timeStampCheckBox, 10, 0, 1, 2);

    // bottom widget
    QWidget *pBottomWgt = new QWidget(this);
    pBottomWgt->setFixedHeight(172);
    QVBoxLayout *pVbottomLyt = new QVBoxLayout(pBottomWgt);
    pVbottomLyt->setContentsMargins(0, 0, 0, 0);
    pMainLyt->addWidget(pBottomWgt);

    QHBoxLayout *pBottomBtnLyt  = new QHBoxLayout;
    pBottomBtnLyt->setContentsMargins(0, 0, 0, 0);
    pVbottomLyt->addLayout(pBottomBtnLyt);

    m_sendSingleMessageBtn = new QPushButton;
    m_sendSingleMessageBtn->setText("单条发送");
    m_sendMultipleMessagesBtn = new QPushButton;
    m_sendMultipleMessagesBtn->setText("多条发送");
    m_protocolTransferBtn = new QPushButton;
    m_protocolTransferBtn->setText("协议传输");
    m_helpBtn = new QPushButton;
    m_helpBtn->setText("帮助");

    pBottomBtnLyt->addWidget(m_sendSingleMessageBtn);
    pBottomBtnLyt->addWidget(m_sendMultipleMessagesBtn);
    pBottomBtnLyt->addWidget(m_protocolTransferBtn);
    pBottomBtnLyt->addWidget(m_helpBtn);
    pBottomBtnLyt->addStretch();

    m_sendSingleMessageWidget = new SendSingleMessageWidget;
    m_sendMultipleMessagesWidget = new SendMultipleMessagesWidget;
    m_protocolTransferWidget = new ProtocolTransferWidget;
    m_helpWidget = new HelpWidget;

    m_stackedWidget = new QStackedWidget(this);
    pVbottomLyt->addWidget(m_stackedWidget);

    m_stackedWidget->addWidget(m_sendSingleMessageWidget);
    m_stackedWidget->addWidget(m_sendMultipleMessagesWidget);
    m_stackedWidget->addWidget(m_protocolTransferWidget);
    m_stackedWidget->addWidget(m_helpWidget);
    m_stackedWidget->setCurrentWidget(m_sendSingleMessageWidget);
}

void SerialPortWidget::connectFun()
{
    connect(m_serialPortOprationBtn, &QPushButton::clicked, this, &SerialPortWidget::serialPortOperation);
    connect(m_clearBtn, &QPushButton::clicked, m_console, &Console::clear);

    // serial port
    // 串口选择
    connect(m_serialPortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SerialPortWidget::showPortInfo);
    connect(m_serialPortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SerialPortWidget::checkCustomDevicePathPolicy);
    // 波特率
    connect(m_baudRateComboBox,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SerialPortWidget::checkCustomBaudRatePolicy);
    // 打开串口
    connect(this, &SerialPortWidget::signOpenSerialPort, m_serialPortInfo, &SerialPortInfo::openSerialPort);
    connect(this, &SerialPortWidget::signCloseSerialPort, m_serialPortInfo, &SerialPortInfo::closeSerialPort);
    connect(m_serialPortInfo, &SerialPortInfo::signReadSerialPortData, this, &SerialPortWidget::showRequest);


    // 单条发送、多条发送、协议传输、帮助
    connect(m_sendSingleMessageBtn, &QPushButton::clicked, this, &SerialPortWidget::pageTurning);
    connect(m_sendMultipleMessagesBtn, &QPushButton::clicked, this, &SerialPortWidget::pageTurning);
    connect(m_protocolTransferBtn, &QPushButton::clicked, this, &SerialPortWidget::pageTurning);
    connect(m_helpBtn, &QPushButton::clicked, this, &SerialPortWidget::pageTurning);
}

void SerialPortWidget::fillPortsParameters()
{
    m_baudRateComboBox->setInsertPolicy(QComboBox::NoInsert);
    m_baudRateComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    m_baudRateComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    m_baudRateComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    m_baudRateComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    m_baudRateComboBox->addItem(tr("Custom"));

    m_stopBitComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    m_stopBitComboBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
    m_stopBitComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    m_dataBitComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    m_dataBitComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    m_dataBitComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    m_dataBitComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

    m_parityCheckComboBox->addItem(tr("None"), QSerialPort::NoParity);
    m_parityCheckComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
    m_parityCheckComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
    m_parityCheckComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    m_parityCheckComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);
}

// 串口选择
void SerialPortWidget::fillPortsInfo()
{
    m_serialPortComboBox->clear();

    QStringList infos = G_GlobalClass->getPortName();

    for (const QString &strInfo : infos)
    {
        QStringList list;
        list << strInfo;
        m_serialPortComboBox->addItem(list.first(), list);
    }
    m_serialPortComboBox->addItem(tr("Custom"));
}

void SerialPortWidget::updateSettings()
{
    // 串口选择
    G_GlobalClass->setPortName(m_serialPortComboBox->currentText());

    // 波特率
    if (m_baudRateComboBox->currentIndex() == 4)
    {
        G_GlobalClass->setBaudRate(m_baudRateComboBox->currentText().toInt());
    }
    else
    {
        G_GlobalClass->setBaudRate(static_cast<QSerialPort::BaudRate>(
                                       m_baudRateComboBox->itemData(m_baudRateComboBox->currentIndex()).toInt()));
    }
    G_GlobalClass->setStringBaudRate(QString::number(G_GlobalClass->serialportData().baudRate));

    // 数据位
    G_GlobalClass->setDataBits(static_cast<QSerialPort::DataBits>(
                                   m_dataBitComboBox->itemData(m_dataBitComboBox->currentIndex()).toInt()));
    G_GlobalClass->setStringDataBits(m_dataBitComboBox->currentText());

    // 奇偶校验
    G_GlobalClass->setParity(static_cast<QSerialPort::Parity>(
                                 m_parityCheckComboBox->itemData(m_parityCheckComboBox->currentIndex()).toInt()));
    G_GlobalClass->setStringParity(m_parityCheckComboBox->currentText());

    // 停止位
    G_GlobalClass->setStopBits(static_cast<QSerialPort::StopBits>(
                                   m_stopBitComboBox->itemData(m_stopBitComboBox->currentIndex()).toInt()));
    G_GlobalClass->setStringStopBits(m_stopBitComboBox->currentText());
}

void SerialPortWidget::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    const QStringList list = m_serialPortComboBox->itemData(idx).toStringList();
}

void SerialPortWidget::serialPortOperation()
{
    static bool isFlag = true;
    if (isFlag)
    {
        updateSettings();

        m_serialPortOprationBtn->setText("关闭串口");
        emit signOpenSerialPort();
    }
    else
    {
        m_serialPortOprationBtn->setText("打开串口");
        emit signCloseSerialPort();
    }
    isFlag = !isFlag;
}

void SerialPortWidget::showRequest(const QByteArray &data)
{
    m_console->putData(data);
}

void SerialPortWidget::pageTurning()
{
    QPushButton *button = dynamic_cast<QPushButton *>(sender());

    if (button == m_sendSingleMessageBtn && m_sendSingleMessageWidget)
    {
        m_stackedWidget->setCurrentWidget(m_sendSingleMessageWidget);
    }
    else if (button == m_sendMultipleMessagesBtn && m_sendMultipleMessagesWidget)
    {
        m_stackedWidget->setCurrentWidget(m_sendMultipleMessagesWidget);
    }
    else if (button == m_protocolTransferBtn && m_protocolTransferWidget)
    {
        m_stackedWidget->setCurrentWidget(m_protocolTransferWidget);
    }
    else if (button == m_helpBtn && m_helpWidget)
    {
        m_stackedWidget->setCurrentWidget(m_helpWidget);
    }
}

void SerialPortWidget::slotClearReceive()
{

}

void SerialPortWidget::checkCustomBaudRatePolicy(int idx)
{
    const bool isCustomBaudRate = !m_baudRateComboBox->itemData(idx).isValid();

    m_baudRateComboBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
    {
        m_baudRateComboBox->clearEditText();
        QLineEdit *edit = m_baudRateComboBox->lineEdit();
        edit->setValidator(m_intValidator);
    }
}

void SerialPortWidget::checkCustomDevicePathPolicy(int idx)
{
    const bool isCustomPath = !m_serialPortComboBox->itemData(idx).isValid();
    m_serialPortComboBox->setEditable(isCustomPath);

    if (isCustomPath)
    {
        m_serialPortComboBox->clearEditText();
    }
}
