#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_PROC 100

/**
 *@file e3a.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que mide el tiempo en crear 100 procesos hijos que calculan los n primeros primos, donde n es un entero
 *pasado como parametro de entrada
 *@param argv[0] el nombre del programa
 *@param argv[1] que sera un entero positivo
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
			
int isprime(int num);

int main(int argc, char **argv){
	
	int i, j;
	int n, c, pid;
	long total;
	struct timeval t0, t1;
	
	if(argc < 2){
	 printf("\nSe espera un entero como argumento\n");
		exit(EXIT_FAILURE);
	}
	gettimeofday(&t0, NULL);	/*!<tomamos la primera referencia de tiempo antes del bucle*/
	
	for(i = 0; i < NUM_PROC; i++){	/*!<bucle que crea los 100 procesos y calcula los n primeros primos*/
		if((pid = fork()) == -1){	
			printf("\nError en FORK\n");
			exit(EXIT_FAILURE);
		}
		
		else if(pid == 0){
			n = atoi(argv[1]);
			c = 0;
			for(j = 3; c < n; j++){
				if(isprime(j)){
					c++;
				}
			}
		exit(EXIT_SUCCESS);
		}
		
		else{
			waitpid(pid, NULL, 0);	
		}
	}
	
	gettimeofday(&t1, NULL);		/*!<tomamos la segunda referencia de tiempo despues del bucle*/
	
	total = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec;	/*!<restamos t1-t0 para calcular tiempo total*/
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
	
	if(num == 2){	/*!<sacamos esta comparacion del bucle para hacerla 1 vez */
		return 1;
	}
	for(int i = 2; i <= lim; i++){
		if(num % i == 0){
			return 0;
		}
	}
	return 1;
}
