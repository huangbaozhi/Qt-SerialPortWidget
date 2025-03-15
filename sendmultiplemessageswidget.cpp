#include "sendmultiplemessageswidget.h"

#include <QLabel>

SendMultipleMessagesWidget::SendMultipleMessagesWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(this);
    label->setText("多条发送");
}
