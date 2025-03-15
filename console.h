#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>

#include <QPlainTextEdit>
#include <QTextEdit>

class Console : public QTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr);

//signals:
//    void getData(const QByteArray &data);

public:
    void putData(const QByteArray &data);
//    void setLocalEchoEnabled(bool set);

//protected:
//    void keyPressEvent(QKeyEvent *e) override;
//    void mousePressEvent(QMouseEvent *e) override;
//    void mouseDoubleClickEvent(QMouseEvent *e) override;
//    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    bool m_localEchoEnabled = false;
};

#endif // CONSOLE_H
