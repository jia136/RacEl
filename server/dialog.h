#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "wiringPi.h"
#include <wiringPiI2C.h>
#include <QDial>
#include <QSlider>
#include <QTimer>
#include <time.h>
#include <QMessageBox>
#define YL 0x48


extern bool flag;
extern bool flag_buzzer;

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    int fd;
    QTime time;
    QString time_alarm;
    QString time_format="hh:mm";  
    bool alarm_activate=0;

    ~Dialog();

public slots: 
    void updateUi(QString);   
    void on_checkBoxLED_stateChanged(int arg1);
    void on_checkBoxBuzzer_stateChanged(int arg1);


private:
    Ui::Dialog *ui;
    QTimer *timer;
};

#endif // DIALOG_H
