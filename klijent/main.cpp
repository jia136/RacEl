#include "dialog.h"
#include <QApplication>
#include <wiringPi.h>

int main(int argc, char *argv[])
{
    wiringPiSetup();
    pinMode(LED, OUTPUT);
    pinMode(MOTION,INPUT);
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
