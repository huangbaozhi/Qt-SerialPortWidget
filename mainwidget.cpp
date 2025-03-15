#include "mainwidget.h"
#include "console.h"
#include "sendsinglemessagewidget.h"
#include "sendmultiplemessageswidget.h"
#include "protocoltransferwidget.h"
#include "helpwidget.h"
#include "serialportinfo.h"
#include "serialportwidget.h"

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

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_intValidator(new QIntValidator(0, 4000000, this))
    , m_serial(new QSerialPort(this))
{

    initSerialPortUi();
    connectFun();

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();

//    QHBoxLayout *pMainLyt = new QHBoxLayout(this);
//    SerialPortWidget *pSerialPortWidget = new SerialPortWidget;
//    pMainLyt->addWidget(pSerialPortWidget);
}

MainWidget::~MainWidget()
{
}

void MainWidget::initUi()
{
    m_serialPortLabel = new QLabel(tr("Serial port:"));

    m_serialPortComboBox = new QComboBox;

    m_waitRequestLabel = new QLabel(tr("Wait request, msec:"));

    m_waitRequestSpinBox = new QSpinBox;
    m_waitRequestSpinBox->setRange(0, 10000);
    m_waitRequestSpinBox->setValue(10000);

    m_responseLabel = new QLabel(tr("Response:"));

    m_responseLineEdit = new QLineEdit(tr("Hello, I'm Slave."));

    m_trafficLabel = new QLabel(tr("No traffic."));

    m_statusLabel = new QLabel(tr("Status: Not running."));

    m_runButton = new QPushButton(tr("Start"));

//    const auto infos = QSerialPortInfo::availablePorts();
//    for (const QSerialPortInfo &info : infos)
//    {
//        m_serialPortComboBox->addItem(info.portName());
//    }

    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(m_serialPortLabel, 0, 0);
    mainLayout->addWidget(m_serialPortComboBox, 0, 1);
    mainLayout->addWidget(m_waitRequestLabel, 1, 0);
    mainLayout->addWidget(m_waitRequestSpinBox, 1, 1);
    mainLayout->addWidget(m_runButton, 0, 2, 2, 1);
    mainLayout->addWidget(m_responseLabel, 2, 0);
    mainLayout->addWidget(m_responseLineEdit, 2, 1, 1, 3);
    mainLayout->addWidget(m_trafficLabel, 3, 0, 1, 4);
    mainLayout->addWidget(m_statusLabel, 4, 0, 1, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("Blocking Slave"));
}

void MainWidget::initSerialPortUi()
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

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        m_serialPortComboBox->addItem(info.portName());
    }

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

void MainWidget::connectFun()
{
    connect(m_baudRateComboBox,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWidget::checkCustomBaudRatePolicy);
    connect(m_serialPortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWidget::checkCustomDevicePathPolicy);

    connect(m_clearBtn, &QPushButton::clicked, m_console, &Console::clear);

    connect(m_sendSingleMessageBtn, &QPushButton::clicked, this, &MainWidget::pageTurning);
    connect(m_sendMultipleMessagesBtn, &QPushButton::clicked, this, &MainWidget::pageTurning);
    connect(m_protocolTransferBtn, &QPushButton::clicked, this, &MainWidget::pageTurning);
    connect(m_helpBtn, &QPushButton::clicked, this, &MainWidget::pageTurning);

    connect(m_sendSingleMessageWidget, &SendSingleMessageWidget::signPeriodMs, this, &MainWidget::slotPeriodMs);
    connect(m_serialPortOprationBtn, &QPushButton::clicked, this, &MainWidget::startSlave);
    connect(&m_thread, &SlaveThread::request, this,&MainWidget::showRequest);
}

void MainWidget::fillPortsParameters()
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
void MainWidget::fillPortsInfo()
{
    m_serialPortComboBox->clear();

    const auto infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos)
    {
        QStringList list;
        list << info.portName();
        m_serialPortComboBox->addItem(list.first(), list);
    }
    m_serialPortComboBox->addItem(tr("Custom"));
}

void MainWidget::updateSettings()
{
    SlaveThread::Settings settings = m_thread.settings();

    // 串口选择
    settings.name = m_serialPortComboBox->currentText();

    // 波特率
    if (m_baudRateComboBox->currentIndex() == 4)
    {
        settings.baudRate = m_baudRateComboBox->currentText().toInt();
    }
    else
    {
        settings.baudRate = static_cast<QSerialPort::BaudRate>(
                    m_baudRateComboBox->itemData(m_baudRateComboBox->currentIndex()).toInt());
    }
    settings.stringBaudRate = QString::number(settings.baudRate);

    // 数据位
    settings.dataBits = static_cast<QSerialPort::DataBits>(
                m_dataBitComboBox->itemData(m_dataBitComboBox->currentIndex()).toInt());
    settings.stringDataBits = m_dataBitComboBox->currentText();

    // 奇偶校验
    settings.parity = static_cast<QSerialPort::Parity>(
                m_parityCheckComboBox->itemData(m_parityCheckComboBox->currentIndex()).toInt());
    settings.stringParity = m_parityCheckComboBox->currentText();

    // 停止位
    settings.stopBits = static_cast<QSerialPort::StopBits>(
                m_stopBitComboBox->itemData(m_stopBitComboBox->currentIndex()).toInt());
    settings.stringStopBits = m_stopBitComboBox->currentText();

    //
}

void MainWidget::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    m_console->setEnabled(false);
}

void MainWidget::startSlave()
{
    m_serialPortOprationBtn->setEnabled(false);
    m_thread.startSlave(m_serialPortComboBox->currentText(),
                        m_periodMs,
                        "Hello, I'm Slave.");
}

void MainWidget::showRequest(const QByteArray &data)
{
    //m_console->setText("111111111111");
    m_console->putData(data);
}

void MainWidget::processError(const QString &s)
{
    activateRunButton();
}

void MainWidget::processTimeout(const QString &s)
{
}

void MainWidget::activateRunButton()
{
    m_serialPortOprationBtn->setEnabled(true);
}

void MainWidget::pageTurning()
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

void MainWidget::slotClearReceive()
{

}

int MainWidget::slotPeriodMs(int ms)
{
    m_periodMs = ms;
}

void MainWidget::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void MainWidget::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    const QStringList list = m_serialPortComboBox->itemData(idx).toStringList();
}

void MainWidget::checkCustomBaudRatePolicy(int idx)
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

void MainWidget::checkCustomDevicePathPolicy(int idx)
{
    const bool isCustomPath = !m_serialPortComboBox->itemData(idx).isValid();
    m_serialPortComboBox->setEditable(isCustomPath);

    if (isCustomPath)
    {
        m_serialPortComboBox->clearEditText();
    }
}

