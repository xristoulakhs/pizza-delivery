#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//oi xronoi einai se lepta
int Ntel=3; //tilefwnites
int Ncook=2;    //psistes
int Noven=10;  //fournoi
int Ndeliverer=7;   //dianomeis
#define Torderlow 1  //elaxisto sto diastima gia paraggelies
#define Torderhigh 5 //megisto 
#define Norderlow 1 //elaxistes pitses
#define Norderhigh 5 //megistes
#define Tpaymentlow 1 //elaxistos xronos gia xrewsi
#define Tpaymenthigh 2 //megistos 
#define Cpizza 10 //kostos pitsas
#define Pfail 0.05 //pithanotita apotuxias xrewsis
#define Tprep 1 //xronos na etoimastei kathe pitsa
#define Tbake 10 //xronos gia na psithei kathei pitsa
#define Tpack 2 //xronos paketarismatos
#define Tdellow 5 //elaxistos xronos paradwsis (to diastima einai tuxaio)
#define Tdelhigh 15 //megistos

unsigned int seed;
int Ncust;

int main()
{
    int Ncust=atoi(argv[1]); /* arithmos pelatwn*/
	  seed=atoi(argv[2]); /*sporos*/
	
    return 0;
}
