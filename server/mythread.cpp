#include "mythread.h"
#include "dialog.h"

MyThread::MyThread(int ID, QString p, QObject *parent) : QThread (parent)
{
    this -> socketDescriptor=ID;
    password = p;
}

void MyThread::run(){
    socket= new QTcpSocket();

    if(!socket -> setSocketDescriptor(this->socketDescriptor)){
        qDebug()<<socketDescriptor<<"Problem setting descriptor";
        return;
    }

    connect(socket,SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()), this, SLOT(disconnected()),Qt::DirectConnection);

    exec();
}

void MyThread::readyRead(){
    QByteArray data = socket->readAll();
    QString recieved_data;

    if(!(QString::compare(QString::fromStdString(data.toStdString()),"0",Qt::CaseInsensitive))){
        recieved_data = "Information: Alarm is deactivated";
        emit event(1);
    }
    else{
        recieved_data = "Information: Alarm is activate, to deactivate enter password:  " + QString::fromStdString(data.toStdString());
    }
    emit event(recieved_data);

}

void MyThread::disconnected()
{
    socket->deleteLater();
    //QString disconnected =QString::number(socketDescriptor) + " Klijent se iskljucio";
    //emit event(disconnected);
    exit(0);
}
