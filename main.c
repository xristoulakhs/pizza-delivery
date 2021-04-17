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

int earnings=0;
float avg_wait=0; //mesos xronos anamonis (epituximenes kai mi)
int max_wait=0; //megistos xronos anamonis (epituximenes kai mi)
float avg_service=0; //mesos xronos eksupiretisis (mono gia epituximenes)
int max_service=0; //megistos (mono gia epituximenes)
float avg_cold=0; //mesos xronos pou kruwnan oi pitses (epituximenes)
int max_cold=0; //megistos (epituximenes)

pthread_cond_t cond;
pthread_mutex_t locktel; //lock gia tilefwnites
pthread_mutex_t lockcook; //lock gia psistes
pthread_mutex_t lockoven; //lock gia fournous
pthread_mutex_t lockdel; //lock gia deliverades

//---inputs----
unsigned int seed;
int Ncust; 
//---------------

void *order(void *x); //tha ulopoiithei meta apla tin ebala gia na mi petaei error sto debug ws twra

int main()
{
    int rc;
    int i;
    
    int Ncust=atoi(argv[1]); /* arithmos pelatwn*/
    seed=atoi(argv[2]); /*sporos*/
    
    if(argc!=3){ //an o xristis dwsei parapanw times
		fprintf(stderr,"Invalid number of parametres!");
		return -1;
	}
	
	if(Ncust<0){ //an dwsei arnitiko arithmo pelatwn
		fprintf(stderr,"Invalid number of customers!");
		return -1;
	}
	
	if(Ncust==0){ //an o arithmos pelatwn einai 0
		printf("No customers!");
		return 0;
	}
	
	pthread_t threads[Ncust];
	int idArray[Ncust];
	pthread_mutex_init(&locktel,NULL);
	pthread_mutex_init(&lockcook,NULL);
	pthread_mutex_init(&lockoven,NULL);
	pthread_mutex_init(&lockdel,NULL);
	pthread_cond_init(&cond,NULL);
	
	for(int i=0;i<Ncust;i++){
	    idArray[i]=i+1;
	    rc=pthread_create(&threads[i],NULL,order,&idArray[i]);
	}
	
	for(int i=0;i<Ncust;i++){
	    pthread_join(threads[i],NULL);
	}
	
	pthread_mutex_destroy(&locktel);
	pthread_mutex_destroy(&lockcook);
	pthread_mutex_destroy(&lockoven);
	pthread_mutex_destroy(&lockdel);
	pthread_cond_destroy(&cond);
	
    return 0;
    
}
