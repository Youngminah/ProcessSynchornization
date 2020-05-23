#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int total_money = 5000000;

void enter_region(){
	asm(
		".data\n"
		"lock:\n"
		".byte 0\n"
		".text\n"
		"_enter_region:\n"
		"movb $1,%al\n"    //move 1to Al
		"xchgb lock,%al\n"
		"cmp $0, %al\n"
		"jne _enter_region\n"
	);
}
void leave_region(){
	asm("movb $0,lock");
}

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
		enter_region();
		printf("[Yougnmin] start its critical section\n");
		critical_region(p);
		printf("[Yougnmin] end its critical section\n");
		leave_region();
		noncritical_region(p);
	}
	return NULL;
}

static void* Lily(void* p){
	int i;
	//while(1){
	for(i=0;i<10;i++){
		puts("[Lily] wait for [Yougnmin]");
		enter_region();
		printf("[Lily] start its critical section\n");
		critical_region(p);
		printf("[Lily] end its critical section\n");
		leave_region();
		noncritical_region(p);	
	}
	return NULL;
}


int main(){
	int rc;

	pthread_t t1,t2;

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
	return 0;
}

