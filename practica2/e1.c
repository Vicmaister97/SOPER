#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 3

void *func1(void *arg){
	char *arg1;
	printf("\nExecuting function 1 before 5s\n");
	arg1 = (char *)arg;
	printf("\nARG = %s\n", arg1);
	sleep(5);
	printf("\nExecuting function 1 after 5s\n");
	pthread_exit(NULL);
}

void *func2(void *arg){
	char *arg1;
	printf("\nExecuting function 2 before 5s\n");
	arg1 = (char *)arg;
	printf("\nARG = %s\n", arg1);
	sleep(5);
	printf("\nExecuting function 2 after 5s\n");
	pthread_exit(NULL);
}

void *func3(void *arg){
	char *arg1;
	printf("\nExecuting function 3 before 5s\n");
	arg1 = (char *)arg;
	printf("\nARG = %s\n", arg1);
	sleep(5);
	printf("\nExecuting function 3 after 5s\n");
	pthread_exit(NULL);
}

int main(){
	
	int t1, t2, t3;
	char *a1 = "a1 me cago", *a2 = "a2 en tu madre", *a3 = "a3 jodeeer";
	pthread_t th1, th2, th3;
	printf("\nGonna create %d threads\n", NUM);
	t1 = pthread_create(&th1, NULL, func1, (void *)a1);
	t2 = pthread_create(&th2, NULL, func2, (void *)a2);
	t3 = pthread_create(&th3, NULL, func3, (void *)a3);
	printf("\nt1 == %d\nt2 == %d\nt3 == %d\n",t1,t2,t3);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	
	printf("\nexiting now\n");
	return 0;
}


/*#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 3

void *func1(){
	char *arg1;
	printf("\nExecuting function 1 before 5s\n");
	//arg1 = (char *)arg;
	//printf("\nARG = %s\n", arg1);
	sleep(5);
	printf("\nExecuting function 1 after 5s\n");
	pthread_exit(NULL);
}

void *func2(){
	char *arg1;
	printf("\nExecuting function 2 before 5s\n");
	//arg1 = (char *)arg;
	//printf("\nARG = %s\n", arg1);
	sleep(5);
	printf("\nExecuting function 2 after 5s\n");
	pthread_exit(NULL);
}

void *func3(){
	char *arg1;
	printf("\nExecuting function 3 before 5s\n");
	//arg1 = (char *)arg;
	//printf("\nARG = %s\n", arg1);
	sleep(5);
	printf("\nExecuting function 3 after 5s\n");
	pthread_exit(NULL);
}

int main(){
	
	int t1, t2, t3;
	char *a1 = "a1 me cago", *a2 = "a2 en tu madre", *a3 = "a3 jodeeer";
	pthread_t th1, th2, th3;
	printf("\nGonna create %d threads\n", NUM);
	t1 = pthread_create(&th1, NULL, func1, NULL);
	t2 = pthread_create(&th2, NULL, func2, NULL);
	t3 = pthread_create(&th3, NULL, func3, NULL);
	printf("\nt1 == %d\nt2 == %d\nt3 == %d\n",t1,t2,t3);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	
	printf("\nexiting now\n");
	return 0;
} */ 
