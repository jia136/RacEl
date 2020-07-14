#include "dialog.h"
#include "ui_dialog.h"


bool flag = true;
bool flag_buzzer = true;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    ui->checkBoxLED->setChecked(true);
    ui->checkBoxBuzzer->setChecked(true);
    timer =new QTimer(this);
    time.setHMS(0,0,0,0);
    timer->start(1000);
    ui->label_6->hide();
    ui->label_3->show();
    ui->light_on->hide();
    ui->light_on_photo->hide();


    ui->motion_on->hide();
    ui->motion_on_photo->hide();


    ui->alarm_on->hide();
    ui->alarm_off->show();
}

void Dialog::updateUi(QString data){
    ui->textEdit->clear();
    ui->textEdit->append(data);

    if(data == "Information: Alarm is deactivated"){
        alarm_activate = 0;
        ui->label_3->show();
        ui->label_6->hide();
        time=time.currentTime();
        delay(1000);
        time_alarm=time.toString(time_format);
        ui->lcdNumber_2->display(time_alarm);

        ui->light_on->hide();
        ui->light_on_photo->hide();

        ui->motion_on->hide();
        ui->motion_on_photo->hide();

        ui->alarm_on->hide();
        ui->alarm_off->show();

    }
    else{
        if(!alarm_activate){
            time=time.currentTime();
            delay(1000);
            time_alarm=time.toString(time_format);
            ui->lcdNumber->display(time_alarm);
            ui->lcdNumber_2->display(0);
            alarm_activate = 1;
        }
        ui->label_3->hide();
        ui->label_6->show();
        if(data.endsWith("Light sensor is activated.")){
            ui->light_on->show();
            ui->motion_on->hide();
            ui->light_on_photo->show();
        }
        else{
            ui->motion_on->show();
            ui->light_on->hide();
            ui->motion_on_photo->show();
        }
        ui->alarm_off->hide();
        ui->alarm_on->show();
    }
}

void Dialog::on_checkBoxLED_stateChanged(int arg1)
{
    if(arg1){
        flag = true;
        qDebug()<<"LED on";
    }
    else{
        flag = false;
        qDebug()<<"LED off";
    }
}

void Dialog::on_checkBoxBuzzer_stateChanged(int arg1)
{
    if(arg1){
        flag_buzzer = true;
        qDebug()<<"Buzzer on";
    }
    else{
        flag_buzzer = false;
        qDebug()<<"Buzzer off";
    }
}

Dialog::~Dialog()
{
    delete ui;
}

