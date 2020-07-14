#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <iostream>
#include <QTimer>
#include <QDateTime>
#include "wiringPi.h"
#include <wiringPiI2C.h>
#include <QSlider>
#include <QMessageBox>
#include <time.h>

#define YL 0x48
#define MOTION 5
#define LED 25

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = nullptr);
    int light;
    int init_light;
    int fd;
    bool status=false;
    bool time_exp=false;
    bool led_state=false;

    bool button_push=false;

    bool alarm_activate_light=0;
    bool alarm_activate_motion=0;
    int current_time;

    QString GetRandomString();
    QString pass;
    QTime time;

    void Connect();
    ~Dialog();

private slots:
    void on_timerUpdate();
    void light_sensor();
    void motion_sensor();
    void alarm_LED();
    void on_pushButton_clicked();//deactivate
    void on_pushButton_2_clicked();//start system
    void on_pushButton_3_clicked();//stop system

private:
    Ui::Dialog *ui;
    QTimer *timer;
};

#endif // DIALOG_H
