#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QPixmap>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int ID,QString p, QObject *parent = nullptr);
    QString pass;

    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void event(QString message);
    void event(bool state);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    QString transaction;
    int socketDescriptor;
    QString password;
};

#endif // MYTHREAD_H
