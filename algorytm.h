#ifndef ALGORYTM_H
#define ALGORYTM_H
#include <QObject>
#include "mainwindow.h"

class algorytm : public QObject
{


private:

    GameLife gra;


public:

    Q_OBJECT

    algorytm();
    void StartGry();

};

#endif // ALGORYTM_H
