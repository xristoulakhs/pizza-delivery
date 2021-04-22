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
int Npack=1; //upalilos paketarismatos
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

int success; //epituximenes paraggelies
int failure; //akurwmenes paraggelies
int earnings=0; //kerdi
float avg_wait=0; //mesos xronos anamonis (epituximenes kai mi)
int max_wait=0; //megistos xronos anamonis (epituximenes kai mi)
float avg_service=0; //mesos xronos eksupiretisis (mono gia epituximenes)
int max_service=0; //megistos (mono gia epituximenes)
float avg_cold=0; //mesos xronos pou kruwnan oi pitses (epituximenes)
int max_cold=0; //megistos (epituximenes)

pthread_cond_t cond_tel= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cook= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_oven= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_del= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_earn= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_avg_wait= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_max_wait= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_avg_service= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_max_service= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_avg_cold= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_max_cold= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_pack= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_success= PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_failure= PTHREAD_COND_INITIALIZER;

pthread_mutex_t locktel= PTHREAD_MUTEX_INITIALIZER; //lock gia tilefwnites
pthread_mutex_t lockcook= PTHREAD_MUTEX_INITIALIZER; //lock gia psistes
pthread_mutex_t lockoven= PTHREAD_MUTEX_INITIALIZER; //lock gia fournous
pthread_mutex_t lockdel= PTHREAD_MUTEX_INITIALIZER; //lock gia deliverades
pthread_mutex_t lockearn= PTHREAD_MUTEX_INITIALIZER; //lock gia earnings
pthread_mutex_t lock_avg_wait= PTHREAD_MUTEX_INITIALIZER; // ktl...
pthread_mutex_t lock_max_wait= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_avg_service= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_max_service= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_avg_cold= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_max_cold= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockpack= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t locksuccess= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockfailure= PTHREAD_MUTEX_INITIALIZER;

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
	
	for(int i=0;i<Ncust;i++){
	    idArray[i]=i+1;
	    rc=pthread_create(&threads[i],NULL,order,&idArray[i]);
	}
	
	for(int i=0;i<Ncust;i++){
	    pthread_join(threads[i],NULL);
	}
	
	avg_cold= avg_cold/Ncust;
	avg_service= avg_service/Ncust;
	avg_wait= avg_wait/Ncust;
	
	printf("The number of successes was %i and the number of failures was %i.\n",success,failure);
	printf("The average waiting time was %f and the max waiting time was %i.\n",avg_wait,max_wait);
	printf("The average service time was %f and the max service time was %i.\n",avg_service,max_service);
	printf("The average pizza colding time was %f and the max time was %i.\n",avg_cold,max_cold);
	
	pthread_mutex_destroy(&locktel);
	pthread_mutex_destroy(&lockcook);
	pthread_mutex_destroy(&lockoven);
	pthread_mutex_destroy(&lockdel);
	pthread_mutex_destroy(&lockearn);
	pthread_mutex_destroy(&lock_avg_wait);
	pthread_mutex_destroy(&lock_max_wait);
	pthread_mutex_destroy(&lock_avg_service);
	pthread_mutex_destroy(&lock_max_service);
	pthread_mutex_destroy(&lock_avg_cold);
	pthread_mutex_destroy(&lock_max_cold);
	pthread_mutex_destroy(&lockpack);
	pthread_mutex_destroy(&locksuccess);
	pthread_mutex_destroy(&lockfailure);
	
	pthread_cond_destroy(&cond_tel);
	pthread_cond_destroy(&cond_cook);
	pthread_cond_destroy(&cond_oven);
	pthread_cond_destroy(&cond_del);
	pthread_cond_destroy(&cond_earn);
	pthread_cond_destroy(&cond_avg_wait);
	pthread_cond_destroy(&cond_max_wait);
	pthread_cond_destroy(&cond_avg_service);
	pthread_cond_destroy(&cond_max_service);
	pthread_cond_destroy(&cond_avg_cold);
	pthread_cond_destroy(&cond_max_cold);
	pthread_cond_destroy(&cond_pack);
	pthread_cond_destroy(&cond_success);
	pthread_cond_destroy(&cond_failure);
	
    return 0;
    
}

void *order(void *x){

    int wait_time;
	int prep_time;
	int deliver_time
	int order_time;
	int cold_time;
	struct timespec start_order, finish_order;
	struct timespec start_wait, finish_wait;
	struct timespec start_cold, finish_cold;
	struct timespec start_prep, finish_prep;
	struct timespec start_del, finish_del;
    int rc;
	unsigned int payment_delay;
	unsigned int pizza;
	unsigned int call_delay;
	unsigned int delivery;
	
	clock_gettime(CLOCK_REALTIME,&start_order);
	
	if(x!=1){
		call_delay=rand_r(&seed) % Torderhigh+ Torderlow;
	}
	else{call_delay=0;}
	
	sleep(call_delay);
	
	clock_gettime(CLOCK_REALTIME,&start_wait);
	
	rc=pthread_mutex_lock(&locktel);
	while(Ntel==0){
		rc=pthread_cond_wait(&cond_tel,&locktel);
	}
	Ntel--;
	rc=pthread_mutex_unlock(&locktel);
	
	clock_gettime(CLOCK_REALTIME,&finish_wait);
	wait_time=finish_wait.tv_sec - start_wait.tv_sec;
	
	if(wait_time>max_wait){
		rc=pthread_mutex_lock(&lock_max_wait);
		max_wait=wait_time;
		rc=pthread_mutex_unlock(&lock_max_wait);
	}
	
	rc=pthread_mutex_lock(&lock_avg_wait);
	avg_wait+= (float) wait_time;
	rc=pthread_mutex_unlock(&lock_avg_wait);
	
	pizza=rand_r(&seed) Norderhigh + Norderlow;
	
	payment_delay(&seed) Tpaymenthigh + Tpaymentlow;
	//sleep(payment_delay);
	
	//na balw kwdika gia pithanotita apotuxias
	//kai auksomeiwsi epituximenwn paraggeliwn
	
	rc=pthread_mutex_lock(&locktel);
	Ntel++;
	rc=pthread_cond_signal(&cond_tel);
	rcpthread_mutex_unlock(&locktel);
	
	printf("Order with id  %i has been registered.\n",x);
	
	rc=pthread_mutex_lock(&lockearn);
	earnings+=Cpizza*pizza;
	rc=pthread_mutex_unlock(&lockearn);
	
	//rc=pthread_mutex_lock(&locksuccess);
	//success++;
	//rc=pthread_mutex_unlock(&locksuccess);
	
	clock_gettime(CLOCK_REALTIME,&start_prep);
	
	rc=pthread_mutex_lock(&lockcook);
	while(Ncook==0){
		rc=pthread_cond_wait(&cond_cook,&lockcook);
	}
	Ncook--;
	rc=pthread_mutex_unlock(&lockcook);
	
	sleep(pizza*Tprep);
	
	rc=pthread_mutex_lock(&lockoven);
	while(Noven<pizza){
		rc=pthread_cond_wait(&cond_oven);
	}
	Noven=Noven - pizza;
	rc=pthread_mutex_unlock(&lockoven);
	
	rc=pthread_mutex_lock(&lockcook);
	Ncook++;
	rc=pthread_cond_signal(&cond_cook);
	rc=pthread_mutex_unlock(&lockcook);
	
	sleep(pizza*Tbake);
	
	clock_gettime(CLOCK_REALTIME,&start_cold);
	
	rc=pthread_mutex_lock(&lockpack);
	while(Npack==0){
		rc=pthread_cond_wait(&cond_pack);
	}
	Npack--;
	rc=pthread_mutex_unlock(&lockpack);
	
	int unpacked=pizza;
	while(unpacked>0){
		rc=pthread_mutex_lock(&lockoven);
		Noven++;
		rc=pthread_cond_signal(&cond_oven);
		rc=pthread_mutex_unlock(&lockoven);
		sleep(Tpack);
		unpacked--;
	}
	
	clock_gettime(CLOCK_REALTIME,&finish_prep);
	prep_time=finish_prep.tv_sec - start_prep.tv_sec;
	printf("Oder with id %i has been prepared in %i minutes\n",x,prep_time);
	
	rc=pthread_mutex_lock(&lockpack);
	Npack++;
	rc=pthread_cond_signal(&cond_pack);
	rc=pthread_mutex_unlock(&lockpack);
	
	clock_gettime(CLOCK_REALTIME,&start_del);
	
	rc=pthread_mutex_lock(&lockdel);
	while(Ndeliverer==0){
		rc=pthread_cond_wait(&cond_del);
	}
	Ndeliverer--;
	rc=pthread_mutex_unlock(&lockdel);
	
	delivery=rand_r(&seed) Tdelhigh + Tdellow;
	
	sleep(delivery);
	
	clock_gettime(CLOCK_REALTIME,finish_del);
	clock_gettime(CLOCK_REALTIME,finish_cold);
	clock_gettime(CLOCK_REALTIME,&finish_order);
	
	deliver_time=finish_del.tv_sec - start_del.tv_sec;
	printf("Order with id %i has beed delivered in %i minutes.\n",x,deliver_time);
	
	cold_time=finish_cold.tv_sec - start_cold.tv_sec;
	if(cold_time>max_cold){
		rc=pthread_mutex_lock(&lock_max_cold);
		max_cold=cold_time;
		rc=pthread_mutex_unlock(&lock_max_cold);
	}
	
	rc=pthread_mutex_lock(&lock_avg_cold);
	avg_cold += (float) cold_time;
	rc=pthread_mutex_unlock(&lock_avg_cold);
	
	order_time=finish_order.tv_sec - start_order.tv_sec;
	if(order_time>max_service){
		rc=pthread_mutex_lock(&lock_max_service);
		max_service=order_time;
		rc=pthread_mutex_unlock(&lock_max_service);
	}
	
	rc=pthread_mutex_lock(&lock_avg_service);
	avg_service+= (float) order_time;
	rc=pthread_mutex_unlock(&lock_avg_service);
	
	sleep(delivery);
	
	rc=pthread_mutex_lock(&lockdel);
	Ndeliverer++;
	rc=pthread_cond_signal(&cond_del);
	rc=pthread_mutex_unlock(&lockdel);
	
	pthread_exit(NULL);
}
