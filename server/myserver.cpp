#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer (parent)
{

    gui = new Dialog;
    gui-> show();

    timer =new QTimer(this);
    timer1= new QTimer(this);

    digitalWrite(LED, LOW);//LED
    digitalWrite(TRIG,LOW);
    digitalWrite(BUZZER,LOW);//BUZZER
    connect(timer1,SIGNAL(timeout()),this,SLOT(distance()));
    connect(timer1,SIGNAL(timeout()),this,SLOT(alarm_LED()));
    connect(timer1,SIGNAL(timeout()),this,SLOT(buzzer()));
    timer->start(1000);
}

void MyServer::startServer(){

    if(!this->listen (QHostAddress::Any,1234)){
        qDebug()<<"Could not start server";
    }
    else{
        qDebug()<<"Server is listening";
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor){
    MyThread *thread = new MyThread(socketDescriptor,password,this);
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));

    connect(thread,SIGNAL(event(QString)),gui,SLOT(updateUi(QString)));

    connect(thread,SIGNAL(event(bool)), this, SLOT(alarm_deactivated()));


    timer1->start(1000);
    thread ->start();
}

void MyServer::alarm_LED(){
    if(flag){
        if((distance_int<80) && (distance_int>=60)){
                softPwmWrite(LED,80);
                delay(10);
        }
        if((distance_int<60) && (distance_int>=40)){
                softPwmWrite(LED,60);
                delay(10);
        }
        else if((distance_int<40) && (distance_int>=20)){
                softPwmWrite(LED,40);
                delay(10);
        }
        else if((distance_int<20) && (distance_int>=10)){
                softPwmWrite(LED,20);
                delay(10);
        }
        else if (distance_int<10) {
                softPwmWrite(LED,0);
                timer1->stop();
                delay(10);
        }
        else {
                softPwmWrite(LED,100);
                delay(10);
        }
    }
    else{
        softPwmWrite(LED,0);
        delay(10);
    }
}


void MyServer::buzzer(){
    /*if(flag_buzzer){
         digitalWrite(BUZZER,HIGH);
    }
    else{
        digitalWrite(BUZZER,LOW);
    }*/
    if(flag_buzzer){
        if((distance_int<80) && (distance_int>=60)){
                softPwmWrite(BUZZER,80);
                delay(10);
        }
        if((distance_int<60) && (distance_int>=40)){
                softPwmWrite(BUZZER,60);
                delay(10);
        }
        else if((distance_int<40) && (distance_int>=20)){
                softPwmWrite(BUZZER,40);
                delay(10);
        }
        else if((distance_int<20) && (distance_int>=10)){
                softPwmWrite(BUZZER,20);
                delay(10);
        }
        else if (distance_int<10) {
                softPwmWrite(BUZZER,0);
                timer1->stop();
                delay(10);
        }
        else {
                softPwmWrite(BUZZER,100);
                delay(10);
        }
    }
    else{
        softPwmWrite(BUZZER,0);
        delay(10);
    }
}

void MyServer::distance(){
        digitalWrite(TRIG,HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG,LOW);

        while(digitalRead(ECHO)==LOW);
        unsigned int startTime_dis = micros();
        while(digitalRead(ECHO)==HIGH);
        unsigned int travelTime_dis = micros() - startTime_dis;

        distance_int = travelTime_dis/59;

        //qDebug()<< "Distance is :" <<distance_int;
 }

void MyServer::alarm_deactivated(){
        softPwmWrite(LED,0);
        softPwmWrite(BUZZER,0);
        timer1->stop();
}
