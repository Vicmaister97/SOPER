#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

#define NUM_THREADS 100
/**
 *@file e3b.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que mide el tiempo en crear 100 hilos que calculan los n primeros primos, donde n es un entero
 *pasado como parametro de entrada
 *@param argv[0] el nombre del programa
 *@param argv[1] que sera un entero positivo
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
	

int isprime(int num);
void *calcprimes(void *num);

int main(int argc, char **argv){

	int i;
	int num;
	long total;
	pthread_t h[NUM_THREADS];
	struct timeval t0, t1;
	
	if(argc < 2){
		printf("\nSe espera un entero como argumento\n");
		exit(EXIT_FAILURE);
	}
	num = atoi(argv[1]);
	
	gettimeofday(&t0, NULL);
	for(i = 0; i < NUM_THREADS; i++){
		
		if(pthread_create(&h[i], NULL, calcprimes, (void *)&num) != 0){	
			printf("\nError en creacion hilo\n");
			exit(EXIT_FAILURE);
		}
		
		else{
			pthread_join(h[i], NULL);
		}
	}
	gettimeofday(&t1, NULL);
	
	total = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec;
	printf("\nTiempo total en uS == %ld\n", total);
	
	exit(EXIT_SUCCESS);
}
		
				
/**
 *@function isprime
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief rutina que calcula si un numero entero pasado por entrada es primo o no
 *@param int num, entero positivo mayor o igual que 2
 *@return 1 si es primo o 0 si no es primo
 *
*/ 
			
int isprime(int num){

	int lim = floor(sqrt(num));
	
	if(num == 2){
		return 1;
	}
	for(int i = 2; i <= lim; i++){
		if(num % i == 0){
			return 0;
		}
	}
	return 1;
}

/**
 *@function calcprimes
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief rutina que calcula los primeros n numeros primos, donde n es un entero pasado por entrada, sera llamada
 *por un hilo
 *@param void *n, entero positivo
 *
*/ 

void *calcprimes(void *n){
	int j, c = 0;
	int num = *(int *)n;
	
	for(j = 2; c < num; j++){
		if(isprime(j)){
			c++;		/*!<aumentamos el contador*/
		}
	}

	pthread_exit(NULL);		/*!<terminamos el hilo*/
}


