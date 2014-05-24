#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QTableWidget>
#include <QLayout>
#include <QSize>
#include<QLCDNumber>
#include <QMouseEvent>
#include <QTimer>
#include <QColorDialog>

class GameLife : public QMainWindow
{
    Q_OBJECT

public:
    GameLife(QWidget *parent = 0);
    ~GameLife();
    QSize sizeHint() const;

    int** nowa_generacja(QTableWidget* tablica); // modyfikacja PolaZycia - dodawanie zyjacych komorek i martwych komorek
private:

    QPushButton *Start;
    QPushButton *Close;
    QPushButton *CleanButton;
    QPushButton *Stop;
    QTableWidget *PoleZycia;
    QLabel *Opcje;
    QLabel *RozmiarKomorki;
    QLabel *LicznikZyjacychNapis;
    QSpinBox *lRozmiarKomorki;
    QVBoxLayout *OpcjeLayout;
    QHBoxLayout *GlownyLayout;
    QHBoxLayout *RKomorkiLayout;
    QHBoxLayout *LicznikLayout;
    QHBoxLayout * KolorLayout;
    QLCDNumber *LicznikZyjacych;
    QTimer * Timer;



protected:
  // virtual void mousePressEvent(QMouseEvent*);


public slots:
    void UstawLKolumniWierszy(int nowa_wartosc);
    void NarodzinyKomorki(QTableWidgetItem *item);
    void SmiercKomorki(QTableWidgetItem *item);
    void ZmienRozmiarKomorki(int nowa_wartosc);
    void ZwiekszLicznik();
    void ZmniejszLicznik();
    void Uruchom();
    void Stworz_Nowe_Pole(int **tablica,QTableWidget *Pole);
    void Clean();
    void InicjalizacjaPolaZycia(int, int);
    void Startuj_Symulacje();
signals:
    void LKolumniWierszyZmieniona(int kolumny, int wiersze);
    void RKomorkiZmieniony(int nowa_wartosc);
    void NowaKomorka();
    void KomorkaUmarla();
    void LicznikZwiekszony(int nowa_wartosc); //nowa_wartosc - obecna wartosc licznika
    void LicznikZmniejszony(int nowa_wartosc);
    void Nowe_Pole_Stworzone(QTableWidget*Pole);
    void Symulacja_Ruszyla();

};



#endif // MAINWINDOW_H
