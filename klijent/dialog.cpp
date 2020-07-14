#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(1000);
    qsrand(QTime::currentTime().msec());

    //light sensor initional value
    fd = wiringPiI2CSetup(YL);
    wiringPiI2CReadReg8(fd, YL);
    init_light = wiringPiI2CReadReg8(fd, YL);
    ui->verticalSlider->setRange(0,255);
    ui->verticalSlider->setEnabled(false);

    //LED off initional
    digitalWrite(LED, LOW);

    //ui start hide all
    ui->alarm_info_on->hide();
    ui->alarm_info_off->hide();
    ui->lcdNumber->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->verticalSlider->hide();
    ui->label_6->hide();
    ui->pushButton_deactivate->hide();
    ui->pushButton_stop->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->bell_on->hide();
    ui->bell_off->hide();
    ui->lineEdit->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->status_light_on->hide();
    ui->status_light_off->hide();
    ui->status_mot_on->hide();
    ui->status_mov_off->hide();


    ui->lineEdit->setEnabled(false);
    ui->pushButton_deactivate->setEnabled(false);
}

void Dialog::light_sensor(){
    light = wiringPiI2CReadReg8(fd, YL);

    ui->verticalSlider->setValue(255-light);

    if(((init_light + 30 < light) || (init_light - 30 > light)) && alarm_activate_light == 0 && (!button_deactivate)){

       alarm_activate_light = 1;//light sensor is activated

       if(!alarm_activate_motion){
            pass = GetRandomString();//generate random string

            time.setHMS(0,0,0,0);
            Connect();

            ui->alarm_info_on->show();
            ui->alarm_info_off->hide();
            ui->bell_on->show();
            ui->bell_off->hide();
            //qDebug()<<pass;
       }

       ui->label_7->hide();//photo_no_light
       ui->label_8->show();//photo_yes_light

       ui->status_light_on->show();
       ui->status_light_off->hide();

    }
}

void Dialog::motion_sensor(){

    if((digitalRead(MOTION)) && alarm_activate_motion == 0 && (!button_deactivate)){
        //qDebug()<<"pokret";
        alarm_activate_motion = 1;
        if(!alarm_activate_light){
            pass = GetRandomString();

            time.setHMS(0,0,0,0);
            Connect();
            //qDebug()<<pass;
            ui->bell_on->show();
            ui->bell_off->hide();
            ui->alarm_info_on->show();
            ui->alarm_info_off->hide();
        }

        ui->label_9->hide();//photo_no_mot
        ui->label_10->show();//photo_yes_mot

        ui->status_mot_on->show();
        ui->status_mov_off->hide();


    }
    /*else{
        qDebug()<<"nema pokreta";
    }*/
}

void Dialog::alarm_LED(){
    //blink
    if(led_state==true){
        digitalWrite(LED, HIGH);
        led_state=false;
    }
    else{
         digitalWrite(LED, LOW);
         led_state=true;
    }

}

QString Dialog::GetRandomString(){
    QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString randomString;
    int stringLength = 5;
    for(int i=0; i<stringLength; ++i){
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

void Dialog::Connect(){
    QTcpSocket *socket = new QTcpSocket();
    socket -> connectToHost("192.168.1.9",1234);
    if(socket->waitForConnected(500)){
        if(alarm_activate_light || alarm_activate_motion){
            socket->write(pass.toUtf8());
            socket->waitForBytesWritten(500);
            if(alarm_activate_motion){
                   socket->write(" Motion sensor is activated.");
                   socket->waitForBytesWritten(500);
            }
            else {
                socket->write(" Light sensor is activated.");
                socket->waitForBytesWritten(500);
            }
        }
        else{
            socket->write("0");//disconnected
            socket->waitForBytesWritten(500);
        }
        socket->close();
    }
    socket ->disconnectFromHost();

    if(socket->state() == QTcpSocket::ConnectedState){
        socket -> waitForDisconnected(1000);
    }
}

void Dialog::on_timerUpdate() {
    if(status){
        light_sensor();
        motion_sensor();

        if(alarm_activate_light == 1 || alarm_activate_motion == 1){

            ui->lineEdit->setEnabled(true);
            ui->pushButton_deactivate->setEnabled(true);

            ui->alarm_info_on->show();
            ui->alarm_info_off->hide();
            alarm_LED();//activate alarm
            time=time.addSecs(1);
            current_time=time.second();

            ui->lcdNumber->display(30-current_time);

            if(current_time>=30 || wrong_pass){
                wrong_pass=false;
                pass = GetRandomString();
                time.setHMS(0,0,0,0);
                Connect();
                //qDebug()<<pass;
            }
        }
    }
}

void Dialog::on_pushButton_deactivate_clicked()
{
    QString enter_pass = ui->lineEdit->text();

    if(enter_pass == pass){
        wrong_pass=false;
        button_deactivate=true;
        alarm_activate_light = 0;
        alarm_activate_motion = 0;
        time.setHMS(0,0,0,0);
        Connect();

        QMessageBox::information(this,"Alarm system","Alarm system is deactivated.");
        ui->lineEdit->clear();

        ui->lcdNumber->display(0);
        ui->label->show();
        ui->bell_on->hide();
        ui->bell_off->show();
        ui->label_8->hide();
        ui->label_7->show();
        ui->label_9->show();
        ui->label_10->hide();
        ui->status_light_on->hide();
        ui->status_light_off->show();
        ui->status_mot_on->hide();
        ui->status_mov_off->show();
        ui->alarm_info_on->hide();
        ui->alarm_info_off->show();
        digitalWrite(LED, LOW);
        button_deactivate=false;

        ui->lineEdit->setEnabled(false);
        ui->pushButton_deactivate->setEnabled(false);
    }
    else{
        ui->lineEdit->clear();
        on_timerUpdate();
        time.setHMS(0,0,0,0);
        wrong_pass=true;
        QMessageBox::warning(this,"Alarm system","Wrong password! Alarm system is not deactivated. ");
    }
}


void Dialog::on_pushButton_start_clicked()
{
    status = true;

    ui->lcdNumber->show();
    ui->label_4->show();//text(Remaining time:)
    ui->label_5->show();//text(Alarm status:)
    ui->verticalSlider->show();
    ui->label_6->show();//text(Motion sensor)
    ui->pushButton_deactivate->show();
    ui->pushButton_stop->show();
    ui->label->show();//text(Light sensor)
    ui->label_2->show();//text(Alarm system)
    ui->bell_on->hide();
    ui->bell_off->show();
    ui->lineEdit->show();
    ui->label_7->show();
    ui->label_8->hide();
    ui->label_9->show();
    ui->label_10->hide();
    ui->status_light_on->hide();
    ui->status_light_off->show();
    ui->status_mot_on->hide();
    ui->status_mov_off->show();
    ui->alarm_info_on->hide();
    ui->alarm_info_off->show();

    ui->pushButton_start->hide();

    ui->lineEdit->setEnabled(false);
    ui->pushButton_deactivate->setEnabled(false);
}

void Dialog::on_pushButton_stop_clicked()
{
    if(alarm_activate_light==0 && alarm_activate_motion==0){
        status = false;
        QMessageBox::information(this,"Alarm system","System is turned off! ");
        ui->alarm_info_on->hide();
        ui->alarm_info_off->hide();
        ui->lcdNumber->hide();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->verticalSlider->hide();
        ui->label_6->hide();
        ui->pushButton_deactivate->hide();
        ui->pushButton_start->show();
        ui->pushButton_stop->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->bell_on->hide();
        ui->bell_off->hide();
        ui->lineEdit->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_9->hide();
        ui->label_10->hide();
        ui->status_light_on->hide();
        ui->status_light_off->hide();
        ui->status_mot_on->hide();
        ui->status_mov_off->hide();
        ui->alarm_info_on->hide();
    }
    else{
        QMessageBox::warning(this,"Alarm system","System cannot be turned off, because alarm is on. First deactivate alarm. ");
    }
}

Dialog::~Dialog()
{
    delete ui;
}
