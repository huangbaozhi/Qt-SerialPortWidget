#include "protocoltransferwidget.h"

#include <QLabel>

ProtocolTransferWidget::ProtocolTransferWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(this);
    label->setText("协议传输");
}
