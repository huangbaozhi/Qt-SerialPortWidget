#include "helpwidget.h"

#include <QLabel>

HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(this);
    label->setText("帮助");
}
