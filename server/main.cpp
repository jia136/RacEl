#include "dialog.h"
#include <QApplication>
#include "myserver.h"
#include "mythread.h"

int main(int argc, char *argv[])
{   
    wiringPiSetup();

    //ultrasonic sensor
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    //LED
    pinMode(LED, OUTPUT);
    softPwmCreate(LED,0,100);

    //buzzer
    pinMode(BUZZER, OUTPUT);
    softPwmCreate(BUZZER,0,100);

    QApplication a(argc, argv);

    MyServer server;
    server.startServer();

    return a.exec();
}
