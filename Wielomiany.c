/**
 * 
 * @file Wielomiany.c
 * @brief Kalkulator wielomianów.
 * 
 * PROGRAM WCZYTUJE CIĄG POLECEŃ SKŁADAJĄCYCH SIĘ ZE ZNAKU OPERACJI
 * ORAZ ZAPISU WIELOMIANU;
 * 
 * PO WCZYTANIU POLECENIA, AKTUALIZUJE WIELOMIAN PRZECHOWYWANY W PAMIĘCI (AKUMULATORZE);
 * 
 * PO AKTUALIZACJI WYPISUJE AKTUALNĄ WARTOŚĆ AKUMULATORA;
 * 
 * DZIAŁANIE PROGRAMU KOŃCZY POLECENIE POSTACI '.\n';
 * 
 * ZAKŁADAMY ŻE PODCZAS DZIAŁANIA PROGRAMU STOPNIE WIELOMIANÓW NIE PRZEKROCZĄ 10 (MAX_ST);
 * 
 * ZAKŁADAMY ŻE WSPÓŁCZYNNIKI MIESZCZĄ SIĘ W TYPIE INT;
 * 
 * @author JAN ROGUWSKI <jroguwski@gmail.com>
 * @date 2021
 * 
 * **/

#include<stdio.h>

/**
 * 
 * DEFINICJE STAŁYCH
 * 
 * **/

/*
Maksymalny stopień wielomianu;
*/
#define MAX_ST 10

/*
Rozmiar tablicy przechowującej wielomiany
*/
#define ROZMIAR ((MAX_ST)+1)

/*
Typ danych - wielomian
jest to tablica o rozmiarze ROZMIAR i elementach typu int;
*/
typedef int WIELOMIAN[ROZMIAR];

/**
 * 
 * WCZYTYWANIE DANYCH
 * 
 * */

/*
Gramatyka opisująca wielomiany:
<wielomian> ::= "0" | [ "-" ] <jednomian> { <operacja> <jednomian> }
<operacja> ::= "+" | "-"
<jednomian> ::= "1" | <dużo> | [ <dużo> ] "x" [ "^" <dużo> ]
<dużo> ::= "1" <cyfra> { <cyfra> } | <cyfra od 2 do 9> { <cyfra> }
<cyfra> ::= "0" | "1" | <cyfra od 2 do 9>
<cyfra od 2 do 9> ::= "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
*/

/*
Funkcja ustawia współczynniki wielomianu w na 0;
tablica w ma ROZMIAR elementów;
*/
void zeruj(WIELOMIAN w)
{
    for(int i=0; i<ROZMIAR; ++i) w[i]=0;
    return;
}

/*
zapisuje na *c następny znak niebędący spacją.
*/
void pobierz_znak(char *c)
{
    char znak=(char)getchar();
    while(znak==' ') znak=(char)getchar();
    *c=znak;
    return;
}

/*
*znak jest cyfrą;
funkcja ustawia *znak na pierwszy znak niebędący spacją PO liczbie;
zapisuje wartość liczby w zmiennej *liczba;
Jeśli *znak nie jest cyfrą, a np. 'x', funkcja ustawi zmienną *liczba na 0, i zwróci niezmieniony *znak;
*/
void czytaj_liczbe(char *znak, int *liczba)
{
    int wynik=0;
    while(*znak>='0' && *znak<='9')
    {
        wynik=wynik*10+(int)(*znak-'0');
        pobierz_znak(znak);
    }
    *liczba=wynik;
    return;
}

/*
znak jest ze zbioru {'+','*'} i oznacza operacje do wykonania w kalkulatorze;
funkcja ustawia *znak na pierwszy znak niebędący spacją PO liczbie (współczynniku);
funkcja nadaje zmiennej *liczba wartość współczynnika wiodącego;
*/
void czytaj_wspolczynnik_wiodacy(char *znak, int *liczba)
{
    pobierz_znak(znak);
    /*
    znak należy do zbioru {'x', [cyfra], '-'};
    */
    int wynik=0;
    if(*znak=='-')
    {
        pobierz_znak(znak);
        /*
        znak należy do zbioru {[cyfra], 'x'};
        */
        if(*znak=='x') wynik=1;
        else czytaj_liczbe(znak, &wynik);
        wynik*=(-1);
    }
    else
    {
        if(*znak=='x') wynik=1;
        else czytaj_liczbe(znak, &wynik);
    }
    *liczba=wynik;
    return;
}

/*
znak jest ze zbioru {'-','+'};
funkcja ustawia *znak na pierwszy znak niebędący spacją PO liczbie - współczynniku (czyli na 'x' albo na '\n');
funkcja zapisuje na zmiennej *współczynnik wartość współczynnika;
*/
void czytaj_wspolczynnik(char *znak, int *wspolczynnik)
{
    int ujemny=(*znak=='-');
    pobierz_znak(znak);
    /*
    znak jest ze zbioru {'x', cyfra};
    */
    czytaj_liczbe(znak, wspolczynnik);
    if(*wspolczynnik==0) *wspolczynnik=1;
    if(ujemny) *wspolczynnik*=(-1);
    return;
}

/*
czyta stopień jednomianu;
znak jest ze zbioru {'x', '\n'};
funkcja ustawia *znak na pierwszy znak niebędący spacją PO liczbie;
f. ustawia *stopien na stopien jednomianu;
*/
void czytaj_stopien(char *znak, int *stopien)
{
    if(*znak=='\n') *stopien=0;
    else//znak=='x'
    {
        pobierz_znak(znak);//znak jest ze zbioru {'+', '-', '^'};
        if(*znak!='^') *stopien=1;
        else
        {
            pobierz_znak(znak);//znak jest cyfrą;
            czytaj_liczbe(znak, stopien);
        }
    }
    return;
}

/*
znak jest ze zbioru {'+', '-'};
funkcja czyta wspolczynnik przy x i stopien jednomianu (czyta też stopień 0);
funkcja ustawia znak na pierwszy znak niebędący spacją PO zapisie jednomianu;
*/
void czytaj_jednomian(char *znak, WIELOMIAN w, int wiodacy)
{
    int wspolczynnik, stopien;
    if(wiodacy) czytaj_wspolczynnik_wiodacy(znak, &wspolczynnik);
    else czytaj_wspolczynnik(znak, &wspolczynnik);
    czytaj_stopien(znak, &stopien);
    w[stopien]=wspolczynnik;
    return;
}

/*
czyta pierwszy znak wiersza polecenia - operację do wykonania w kalkulatorze;
Zapisuje znak ten do zmiennej *operacja;
*/
void czytaj_operacje(char *operacja)
{
    pobierz_znak(operacja);
    return;
}

/*
zeruje współczynniki wielomianu w;
wczytuje do niej wielomian, zapisując w odpowiednich komórkach współczynniki;
indeks w tablicy odpowiada stopniowi jednomianu;
Funkcja kończy działanie po napotkaniu znaku końca wiersza;
*/
void czytaj_wielomian(WIELOMIAN w)
{
    zeruj(w);
    char znak;
    czytaj_jednomian(&znak, w, 1);//sczytaj jednomian wiodący;
    while(znak!='\n') czytaj_jednomian(&znak, w, 0);
    return;
}

/*
Funkcja czyta jeden wiersz polecenia;
najpierw wczytywana jest operacja - znak ze zbioru {'+','-','.'};
jeśli wczytana zostanie '.', zmienna koniec przyjmuje wartość 1;
jest to sygnał, że nastąpił koniec danych wejściowych;
*/
void czytaj_polecenie(char *operacja, WIELOMIAN w, int *koniec)
{
    czytaj_operacje(operacja);
    if(*operacja!='.') czytaj_wielomian(w);
    else *koniec=1;
    return;
}

/**
 * 
 * WYPISYWANIE
 * 
 * **/

/*
Wartość bezwzględna - oblicza moduł z liczby;
*/
int abs(int x)
{
    if(x>0) return x;
    else return (-1)*x;
}

/*
pisze współczynnik jednomianu, zgodnie z zadaną gramatyką;
*/
void pisz_wspolczynnik(int liczba, int wiodacy, int stopien)//liczba!=0
{
    char znak;
    int dodatnia=(liczba>0);
    if(dodatnia) znak='+';
    else znak='-';
    int modul=abs(liczba);
    if(wiodacy)
    {
        if(stopien==0) printf("%d", liczba);//chcemy wypisac liczbe, jesli jest rowna +-1;
        else//nie chcemy wypisywac +-1;
        {
            if(liczba==(-1)) printf("%c", znak);//+-x
            else if(liczba!=1) printf("%d", liczba);
        }
    }
    else
    {
        if(stopien==0) printf(" %c %d", znak, modul);//chcemy wypisac liczbe jesli jest +-1;
        else//nie chcemy wypisywac liczby +-1;
        {
            if(modul==1)
            {
                printf(" %c ", znak);
            }
            else printf(" %c %d", znak, modul);
        }
    }
    return;
}

/*
pisze x i odpowiednia potege, zgodnie z zadaną gramatyką;
*/
void pisz_x(int stopien)
{
    if(stopien>0)
    {
        if(stopien==1) printf("x");
        else printf("x^%d", stopien);
    }
    return;
}

/*
wypisuje wielomian zerowy: "0";
*/
void pisz_zerowy()
{
    printf("0");
    return;
}

/*
wypisuje na wyjście dany wielomian, zgodnie z daną w zadaniu gramatyką;
wypisanie wielomianu kończy wypisaniem znaku '\n';
*/
void pisz_wielomian(WIELOMIAN wspol)
{
    int wiodacy=1;
    for(int i=MAX_ST; i>=0; i--)
    {
        if(wspol[i]!=0)
        {
            pisz_wspolczynnik(wspol[i], wiodacy, i);//podajemy niezerowy wspolczynnik do wypisania i informacje o tym, czy jest to wspolczynnnik wiodący
            wiodacy=0;
            pisz_x(i);
        }
    }
    if(wiodacy) pisz_zerowy();
    printf("\n");
    return;
}

/**
 * 
 * WYKONYWANIE OBLICZEŃ
 * 
 * **/

/*
Dodaje do akumulatora wielomian w, wynik zapisuje w akumulatorze;
*/
void suma(WIELOMIAN w, WIELOMIAN ak)
{
    for(int i=0; i<ROZMIAR; ++i) ak[i]+=w[i];
    return;
}

/*
oblicza iloczyn wielomianów w*ak, zapisuje do wielomianu wynik;
następnie aktualizuje akumulator;
*/
void iloczyn(WIELOMIAN w, WIELOMIAN ak)
{
    WIELOMIAN wynik;
    zeruj(wynik);
    /*
    obliczamy iloczyn wielomianów w*ak;
    */
    for(int i=0; i<ROZMIAR; i++)
    {
        for(int j=0; j<ROZMIAR; j++)
        {
            if(w[i]!=0 && ak[j]!=0) wynik[i+j]+=w[i]*ak[j];
        }    
    }
    for(int i=0;i<ROZMIAR; ++i) ak[i]=wynik[i];
    return;
}

/*
wykonuje operację *operacja ze zbioru {'+','*'};
*/
void wykonaj_operacje(WIELOMIAN w, WIELOMIAN ak, char *operacja)
{
    switch(*operacja)
    {
        case '+':
        {
            suma(w,ak);
            break;
        }
        case '*':
        {
            iloczyn(w,ak);
            break;
        }
    }
    return;
}

int main()
{
    /*
    Flaga - sygnalizator końca danych wejściowych;
    */
    int koniec=0;
    
    char operacja;
    WIELOMIAN wielomian, akumulator;
    
    /*
    inicjalizacja akumulatora na wielomian zerowy;
    */
    zeruj(akumulator);

    czytaj_polecenie(&operacja, wielomian, &koniec);
    while(!koniec)
    {
        wykonaj_operacje(wielomian, akumulator, &operacja);
        pisz_wielomian(akumulator);
        czytaj_polecenie(&operacja, wielomian, &koniec);
    }
    return 0;
}
