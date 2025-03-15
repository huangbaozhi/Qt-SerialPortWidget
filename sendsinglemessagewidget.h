#ifndef SENDSINGLEMESSAGEWIDGET_H
#define SENDSINGLEMESSAGEWIDGET_H

#include <QWidget>
#include <QTextEdit>

class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QScrollArea;
class QProgressBar;

class SendSingleMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SendSingleMessageWidget(QWidget *parent = nullptr);

signals:
    int signPeriodMs(int ms);

private:
    void initUi();
    void connectFun();

private slots:
   void slotPeriodMs(QString period);

private:
    QPushButton *m_pSendBtn;
    QPushButton *m_pClearBtn;

    QPushButton *m_pOpenFileBtn;
    QPushButton *m_pSendFileBtn;
    QPushButton *m_pStopSendBtn;

    QCheckBox   *m_pTimedTransmissionCheckBox;  // 定时发送
    QCheckBox   *m_pHexSendCheckBox;            // 十六进制
    QCheckBox   *m_pSendNewLineCheckBox;        // 发送新行

    QLineEdit   *m_pFilePathLbl;
    QTextEdit   *m_pSendMessageTextEdit;
    QLineEdit   *m_pPeriodLineEdit;
    QProgressBar *m_pProgressBar;
};

#endif // SENDSINGLEMESSAGEWIDGET_H
