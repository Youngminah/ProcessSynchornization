#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int total_money = 5000000;
sem_t bin_sem;


void critical_region(char *p){
	int local_money;
	int d =rand()%1000000;
	local_money = total_money;
	usleep(10);
	local_money+=d;
	usleep(10);
	total_money = local_money;
	printf("%s customer : Deposit %d won <WOORI BANK>,[TOTAL BALANCE: %d]\n", p, d, total_money);
	printf("[%s] sleep %d microsecond in critical section\n",p,d);
	usleep(d);
	local_money = total_money;	
	usleep(10);
	local_money -= d;
	usleep(10);
	total_money = local_money;
	printf("%s customer : Withdraw %d won <WOORI BANK>,[TOTAL BALANCE: %d]\n", p, d, total_money);
}

void noncritical_region(char *p){
	int d = rand()%1000000;
	printf("[%s] sleep %d microsecond in NON-critical section\n",p,d);
	usleep(d);
}


static void* Yougnmin(void*p){
	int i;
	//while(1){
	for(i=0;i<10;i++){
		puts("[Yougnmin] wait for [Lily]");
		sem_wait(&bin_sem);
		printf("[Yougnmin] start its critical section\n");
		critical_region(p);
		printf("[Yougnmin] end its critical section\n");
		sem_post(&bin_sem);
		noncritical_region(p);
	}
	return NULL;
}

static void* Lily(void* p){
	int i;
	//while(1){
	for(i=0;i<10;i++){
		puts("[Lily] wait for [Yougnmin]");
		sem_wait(&bin_sem);
		printf("[Lily] start its critical section\n");
		critical_region(p);
		printf("[Lily] end its critical section\n");
		sem_post(&bin_sem);
		noncritical_region(p);	
	}
	return NULL;
}


int main(){
	int rc;

	pthread_t t1,t2;

	sem_init(&bin_sem,0,0);
	sem_post(&bin_sem);
	rc = pthread_create(&t1,NULL,Yougnmin,"Yougnmin");

	if(rc!=0){
		fprintf(stderr,"pthread Yougnmin failed\n");
		return EXIT_FAILURE;
	}
	rc = pthread_create(&t2, NULL, Lily,"Lily");
	if(rc !=0){
		fprintf(stderr,"pthread Lily failed\n");
		return EXIT_FAILURE;
	}

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	puts("");
	puts("");
	printf("------------------------<WOORI BANK>-----------------------\n", total_money);
	printf("             FINAL TOTAL BALANCE :  %d (won)\n", total_money);

	puts("\n\nAll threads finished.");
	sem_destroy(&bin_sem);
	return 0;
}

