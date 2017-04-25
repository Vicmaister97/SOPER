#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>

/**
 *@file ejercicio4.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que solicita dimension, dos multiplicadores y dos matrices cuadradas para multiplicar
 *mediante el uso de 2 hilos
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
void *multmatrices(void *mat);

typedef struct _Arg arg;
struct _Arg{
	int dim;
	int mult;
	int *matriz;
	int thread;
	int fila;
	arg *argo;
};

int main(void){
	int dim = 0, size, mult1, mult2;
	int i;
	arg *matriz1, *matriz2;
	int **arg1, **arg2;
	pthread_t h1, h2;
	
	while(dim < 1 || dim > 4){
		printf("\nIntroduzca dimension de la matriz cuadrada:\n");
		scanf("%d", &dim);
		if(dim < 1){
			printf("\nLa dimension tiene que ser en un entero positivo\n");
		}
		if(dim > 4){
			printf("\nLa dimension no puede exceder 4\n");
		}
	}
	matriz1 = (arg *)malloc(sizeof(arg));
	matriz2 = (arg *)malloc(sizeof(arg));
	matriz1->dim = dim;
	matriz2->dim = dim;
	matriz1->thread = 1;
	matriz1->argo = matriz2;
	matriz2->thread = 2;
	matriz2->argo = matriz1;
	
	printf("\nIntroduzca multiplicador 1:\n");
	scanf("%d", &matriz1->mult);
	printf("\nIntroduzca multiplicador 2:\n");
	scanf("%d", &matriz2->mult);
	
	printf("\nIntroduzca la matriz 1:\n");
	size = dim*dim;
	matriz1->matriz = (int *)malloc(sizeof(int) * size);
	for(i = 0; i < size; i++){
		scanf("%d", &matriz1->matriz[i]);
	}
	
	printf("\nIntroduzca la matriz 2:\n");
	matriz2->matriz = (int *)malloc(sizeof(int) * size);
	for(i = 0; i < size; i++){
		scanf("%d", &matriz2->matriz[i]);
	}

	if(pthread_create(&h1, NULL, multmatrices, (void *)matriz1) != 0){	
		printf("\nError en creacion hilo\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_create(&h2, NULL, multmatrices, (void *)matriz2) != 0){	
		printf("\nError en creacion hilo\n");
		exit(EXIT_FAILURE);
	}
	
	pthread_join(h1, NULL);		/*!<el hilo principal espera a los 2 creados*/
	pthread_join(h2, NULL);
	
	free(matriz1->matriz);
	free(matriz2->matriz);
	free(matriz1);
	free(matriz2);
	
	exit(EXIT_SUCCESS);

}

/**
 *@function multmatrices
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief multiplica 1 matriz por un entero e imprime el resultado fila a fila, comprobando el estado de otro hilo
 *que esta haciendo la misma operacion
 *
 *@param void *mat, estructura arg
 *@return pthread_exit(NULL) terminacion del hilo
 *
*/ 
void *multmatrices(void *mat){
	int i, j;
	int size;
	arg *m = (arg *)mat;
	size = m->dim * m->dim;
	m->fila = 0;
	char *a = NULL, *s = NULL;
	
	for(i = 0; i < size; i++){
		m->matriz[i] = m->matriz[i] * m->mult;

		if(i != 0 && ((i + 1) % m->dim == 0)){	/*!<solo ocurre 1 vez por fila de la matriz*/
			s = (char *)malloc(sizeof(char) * 100);
			sprintf(s,"\nHilo %d multiplicando fila %d resultado: ", m->thread, m->fila);
			for(j = 0 ; j < m->dim; j++){
				a = (char *)malloc(sizeof(char) * 100);
				sprintf(a,"%d ", m->matriz[m->fila * m->dim + j]);
				strcat(s,a);
				free(a);
			}
			a = (char *)malloc(sizeof(char) * 100);
 			sprintf(a," - EL hilo %d va por la fila %d\n", m->argo->thread, m->argo->fila);
 			strcat(s,a);
 			free(a);
 			printf("%s",s);		/*!<contiene toda la linea a imprimir cada vez*/
 			free(s);

			sleep(2);	/*!<pausamos durante 2s*/
			m->fila++;
		}
	}
	pthread_exit(NULL);
}
	
	
	
		