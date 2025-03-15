#include "sendsinglemessagewidget.h"

#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include <QLineEdit>
#include <QScrollArea>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

SendSingleMessageWidget::SendSingleMessageWidget(QWidget *parent) : QWidget(parent)
{
    initUi();
    connectFun();
}

void SendSingleMessageWidget::initUi()
{
    QVBoxLayout *pMainLyt = new QVBoxLayout(this);
    pMainLyt->setContentsMargins(5, 0, 5, 5);

    QHBoxLayout *pTopLyt = new QHBoxLayout;
    pTopLyt->setContentsMargins(0, 0, 0, 0);
    m_pSendMessageTextEdit = new QTextEdit;
    m_pSendMessageTextEdit->setFixedHeight(80);

    QVBoxLayout *pRightBtnLyt = new QVBoxLayout;
    pRightBtnLyt->setContentsMargins(0, 0, 0, 0);
    m_pSendBtn = new QPushButton;
    m_pSendBtn->setText("发送");
    m_pClearBtn = new QPushButton;
    m_pClearBtn->setText("清除发送");

    pRightBtnLyt->addWidget(m_pSendBtn);
    pRightBtnLyt->addWidget(m_pClearBtn);

    pTopLyt->addWidget(m_pSendMessageTextEdit);
    pTopLyt->addLayout(pRightBtnLyt);

    QHBoxLayout *pHLyt = new QHBoxLayout;
    pHLyt->setContentsMargins(0, 0, 0, 0);
    m_pTimedTransmissionCheckBox = new QCheckBox;
    m_pTimedTransmissionCheckBox->setText("定时发送");

    QWidget *pPeriodWgt = new QWidget(this);
    QHBoxLayout *pPeriodLyt = new QHBoxLayout(pPeriodWgt);
    pPeriodLyt->setContentsMargins(0, 0, 0, 0);
    QLabel *pPeriodLbl = new QLabel;
    pPeriodLbl->setText("周期:");
    m_pPeriodLineEdit = new QLineEdit;
    m_pPeriodLineEdit->setFixedWidth(34);
    m_pPeriodLineEdit->setText("1000");

    QLabel *pMsLbl = new QLabel;
    pMsLbl->setText("ms");
    pPeriodLyt->addWidget(pPeriodLbl);
    pPeriodLyt->addWidget(m_pPeriodLineEdit);
    pPeriodLyt->addWidget(pMsLbl);
    pPeriodLyt->addStretch();

    m_pFilePathLbl = new QLineEdit;
    m_pFilePathLbl->setMinimumSize(132, 17);
    m_pOpenFileBtn = new QPushButton;
    m_pOpenFileBtn->setText("打开文件");
    m_pSendFileBtn = new QPushButton;
    m_pSendFileBtn->setText("发送文件");
    m_pStopSendBtn = new QPushButton;
    m_pStopSendBtn->setText("停止发送");

    pHLyt->addWidget(m_pTimedTransmissionCheckBox);
    pHLyt->addWidget(pPeriodWgt);
    pHLyt->addWidget(m_pFilePathLbl);
    pHLyt->addWidget(m_pOpenFileBtn);
    pHLyt->addWidget(m_pSendFileBtn);
    pHLyt->addWidget(m_pStopSendBtn);

    QHBoxLayout *pHlayout = new QHBoxLayout;
    pHlayout->setContentsMargins(0, 0, 0, 0);
    m_pHexSendCheckBox = new QCheckBox;
    m_pHexSendCheckBox->setText("16进制发送");
    m_pSendNewLineCheckBox = new QCheckBox;
    m_pSendNewLineCheckBox->setText("发送新行");
    m_pProgressBar = new QProgressBar;

    pHlayout->addWidget(m_pHexSendCheckBox);
    pHlayout->addWidget(m_pSendNewLineCheckBox);
    pHlayout->addWidget(m_pProgressBar);

    pMainLyt->addLayout(pTopLyt);
    pMainLyt->addLayout(pHLyt);
    pMainLyt->addLayout(pHlayout);
}

void SendSingleMessageWidget::connectFun()
{
    connect(m_pPeriodLineEdit, &QLineEdit::textChanged, this, &SendSingleMessageWidget::slotPeriodMs);
}

void SendSingleMessageWidget::slotPeriodMs(QString period)
{
    emit signPeriodMs(period.toInt());
}
