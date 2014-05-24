#include "mainwindow.h"
#include <QApplication>
#include<iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameLife w;
    w.show();
    return a.exec();
}
