#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include "dialog.h"
#include "mythread.h"
#include "softPwm.h"

#define BUZZER 1
#define LED 4
#define TRIG 5
#define ECHO 6


class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void startServer();
    QString password;
    unsigned int distance_int;
    bool flag_myServer;

public slots:
    void alarm_LED();
    void buzzer();
    void distance();
    void alarm_deactivated();

protected:
    Dialog *gui;
    void incomingConnection(qintptr socketDescriptor);
    QTimer *timer;
    QTimer *timer1;
};

#endif // MYSERVER_H
