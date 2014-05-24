#include "mainwindow.h"
#include<QColor>
#include<QHeaderView>
#include <QSize>
#include<QBrush>
#include<QFont>
#include <cmath>

GameLife::GameLife(QWidget *parent)
    : QMainWindow(parent)
{
//Tworze przyciski, Tablice, Napis Kolumny i Wiersze, licznik kolumn i wierszy, i ustawiam w layouty
    Start = new QPushButton("ROZPOCZNIJ");
    Close = new QPushButton("ZAMKNIJ");
    CleanButton = new QPushButton("WYCZYŚĆ");
    Stop = new QPushButton("STOP");
    PoleZycia = new QTableWidget(this);
    PoleZycia->horizontalHeader()->hide();// ukrywa naglowki kolumni i wierszy
    PoleZycia->verticalHeader()->hide();
    ZmienRozmiarKomorki(10);
    PoleZycia->setColumnCount(70);
    PoleZycia->setRowCount(49);
    PoleZycia->setShowGrid(false);
    Opcje = new QLabel ("Opcje:");
    RozmiarKomorki = new QLabel ("Rozmiar komórki:");
    LicznikZyjacychNapis = new QLabel("Zyjacych:");
    lRozmiarKomorki = new QSpinBox;
    OpcjeLayout = new QVBoxLayout;
    GlownyLayout = new QHBoxLayout;
    RKomorkiLayout = new QHBoxLayout;
    LicznikLayout = new QHBoxLayout;
    LicznikZyjacych = new QLCDNumber;
    Timer = new QTimer;


    RKomorkiLayout->addWidget(RozmiarKomorki); // ustawiam wdgety do layoutu dotyczacego rozmiaru komorki
    RKomorkiLayout->addWidget(lRozmiarKomorki);

    LicznikLayout->addWidget(LicznikZyjacychNapis); // ustawiam widgety do layoutu dotyczacego licznika zyjacych komorek
    LicznikLayout->addWidget(LicznikZyjacych);


    OpcjeLayout->addWidget(Opcje); // dodaje napis " Opcje"
    OpcjeLayout->addLayout(RKomorkiLayout); // widget do zmiany rozmiaru
    OpcjeLayout->addLayout(LicznikLayout); // licznik zyjacych
    OpcjeLayout->addStretch(); // przerwa
    OpcjeLayout->addWidget(Start); // przycisk startowy
    OpcjeLayout->addWidget(Stop); // przycisk stopu
    OpcjeLayout->addWidget(CleanButton); // przycisk do czyszczenia tablicy
    OpcjeLayout->addWidget(Close); // przycisk do zamkniecia aplikacji


    GlownyLayout->addWidget(PoleZycia); // ustawianie glownego layoutu
    GlownyLayout->addLayout(OpcjeLayout);


    QWidget *mycentralWidget = new QWidget; // ustawiam widget jako centralny widget mojego MainWindow
    mycentralWidget->setLayout(GlownyLayout);
    this->setCentralWidget(mycentralWidget);


    connect(Close, SIGNAL(clicked()),this, SLOT(close())); // przycisk zamkniecia
    connect(lRozmiarKomorki,SIGNAL(valueChanged(int)),this, SLOT(ZmienRozmiarKomorki(int))); // zmiana w widgecie rozmiaru, powoduje zmiane rozmiaru komorki
    connect(PoleZycia,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(NarodzinyKomorki(QTableWidgetItem*))); // zmienia kolor po kliknieciu na czerwony
    connect(this, SIGNAL(RKomorkiZmieniony(int)),this,SLOT(UstawLKolumniWierszy(int))); //dostosowanie liczby kolumn
    connect(this,SIGNAL(LKolumniWierszyZmieniona(int,int)),this,SLOT(InicjalizacjaPolaZycia(int, int)));
    connect(this,SIGNAL(NowaKomorka()),this, SLOT(ZwiekszLicznik()));
    connect(this,SIGNAL(KomorkaUmarla()),this,SLOT(ZmniejszLicznik()));
    connect(Start,SIGNAL(clicked()),this,SLOT(Startuj_Symulacje()));
    connect(Timer,SIGNAL(timeout()),this, SLOT(Uruchom()));
    connect(Stop,SIGNAL(clicked()),Timer,SLOT(stop()));
    connect(CleanButton,SIGNAL(clicked()),this,SLOT(Clean()));


    for ( int i=0; i <PoleZycia->rowCount();i++)
    {
        for (int j=0; j <PoleZycia->columnCount();j++)
        {
           QTableWidgetItem *item = new QTableWidgetItem;
           item->setBackgroundColor(Qt::black);
           PoleZycia->setItem(i,j,item); // dodanie itemow do tablicy
           SmiercKomorki(PoleZycia->item(i,j));

        }
    }
   }


QSize GameLife::sizeHint() const
{
        return QSize(2000, 2000); // domyslny rozmiar okna
}

void GameLife::Clean()
{

    for ( int i=0; i <PoleZycia->rowCount();i++)
    {
        for (int j=0; j <PoleZycia->columnCount();j++)
        {
           SmiercKomorki(PoleZycia->item(i,j));

        }
    }

}

/*void GameLife::mousePressEvent(QMouseEvent *event) //dlaczego nie działa ???s
{

    if(event->button()== Qt::LeftButton)
    {
        int x=event->x();
        int y=event->y();
        NarodzinyKomorki(PoleZycia->item(x,y));
    }
}*/

/*void GameLife::mousePressEvent(QMouseEvent *e)
{
    int rozmiarkomorki = PoleZycia->horizontalHeader()->defaultSectionSize();

    int k = floor(e->y()/rozmiarkomorki)+1;
    int j = floor(e->x()/rozmiarkomorki)+1;
    NarodzinyKomorki(PoleZycia->item(k,j));
}*/



void GameLife::InicjalizacjaPolaZycia(int kolumny, int wiersze)
{
    for ( int i=0; i <wiersze;i++)
    {
        for (int j=0; j <kolumny;j++)
        {
           QTableWidgetItem *item = new QTableWidgetItem;
           item->setBackgroundColor(Qt::black);
           PoleZycia->setItem(i,j,item); // dodanie itemow do tablicy
           SmiercKomorki(PoleZycia->item(i,j));
           LicznikZyjacych->display(0);

        }
    }
}




void GameLife::ZmienRozmiarKomorki(int nowa_wartosc)
{

     PoleZycia->horizontalHeader()->setDefaultSectionSize(nowa_wartosc);
     PoleZycia->verticalHeader()->setDefaultSectionSize(nowa_wartosc);

     emit RKomorkiZmieniony(nowa_wartosc);


}

//Ustawianie dynamiczne, podczas wykonywania programu

void GameLife::UstawLKolumniWierszy(int nowa_wartosc) // dopasowuje rozmiar pola zycia
{

    int kolumny;
    int wartosc_zmieniona;
    kolumny = PoleZycia->columnCount();
    wartosc_zmieniona = (700-(700%nowa_wartosc))/nowa_wartosc;
    if(kolumny!=wartosc_zmieniona)
    {
        PoleZycia->setColumnCount(wartosc_zmieniona);

    }
    int wiersze;
    int wartosc_zmieniona1;
    wiersze = PoleZycia->rowCount();
    wartosc_zmieniona1=(490-(490%nowa_wartosc))/nowa_wartosc;
    if(wiersze!=wartosc_zmieniona1)
    {
        PoleZycia->setRowCount(wartosc_zmieniona1);

    }
    emit LKolumniWierszyZmieniona(wartosc_zmieniona,wartosc_zmieniona1);
}



void GameLife::NarodzinyKomorki(QTableWidgetItem *item)
{

    if(item->data(Qt::UserRole)!=true) // tak zeby po kolejnym kliknieciu na juz zywa komorke nie wywolywac funkcji
    {
        bool stan;
        stan=true;
        item->setBackgroundColor(Qt::green);
        item->setData(Qt::UserRole,stan );
        emit NowaKomorka();
    }


}

void GameLife::SmiercKomorki(QTableWidgetItem *item)
{
    if(item->data(Qt::UserRole)==true) // tak zeby po kolejnym kliknieciu na juz zywa komorke nie wywolywac funkcji
    {
        bool stan;
        stan=false;
        item->setBackgroundColor(Qt::black);
        item->setData(Qt::UserRole,stan );
        emit KomorkaUmarla();
    }

}


void GameLife::ZwiekszLicznik()
{
    int wartosc = LicznikZyjacych->value();
    int nowa_wartosc = wartosc+1;
    LicznikZyjacych->display(nowa_wartosc);
    emit LicznikZwiekszony(nowa_wartosc);

}


void GameLife::ZmniejszLicznik()
{
    int wartosc = LicznikZyjacych->value();
    int nowa_wartosc = wartosc-1;
    LicznikZyjacych->display(nowa_wartosc);
    emit LicznikZmniejszony(nowa_wartosc);

}

void GameLife::Uruchom()
{

            int ** tymcz = nowa_generacja(PoleZycia);
            Stworz_Nowe_Pole(tymcz,PoleZycia);

}

void GameLife::Startuj_Symulacje()
{
    Timer->start(50);
           emit Symulacja_Ruszyla();
}

int** GameLife::nowa_generacja(QTableWidget *tablica)
{


    int wiersze = tablica->rowCount();
    int kolumny = tablica->columnCount();
    int **tymcz = new int *[wiersze];

    for(int i =0; i<wiersze; i++)
    {
        tymcz[i]= new int[kolumny];
    }

    for(int i=1; i <wiersze-1;i++)
    {
        for(int j=1; j <kolumny-1; j++)
        {
            int ZywiSasiedzi=0;
            if(tablica->item(i+1,j)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i,j+1)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i+1,j+1)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i-1,j)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i,j-1)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i-1,j-1)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i+1,j-1)->data(Qt::UserRole)==true) ++ZywiSasiedzi;
            if(tablica->item(i-1,j+1)->data(Qt::UserRole)==true) ++ZywiSasiedzi;

            tymcz[i][j]=ZywiSasiedzi;
          }
    }

    return tymcz;
}

void GameLife::Stworz_Nowe_Pole(int **tablica,QTableWidget *Pole)
{

    for(int i=1; i < Pole->rowCount()-1; i++)
        for (int j=1; j <Pole->columnCount(); j++)
        {
            QTableWidgetItem *current = Pole->item(i,j);
            if(current->data(Qt::UserRole)!=true)
            {
                if(tablica[i][j]==3)
                {
                    NarodzinyKomorki(current);
                }
            }
            else if(current->data(Qt::UserRole)==true)
            {
                if(tablica[i][j]==0 || tablica[i][j]==1)
                {
                    SmiercKomorki(current);
                }
                else if(tablica[i][j]>3)
                {
                    SmiercKomorki(current);
                }
            }
        }

    emit Nowe_Pole_Stworzone(Pole);
}

GameLife::~GameLife()
{

}
