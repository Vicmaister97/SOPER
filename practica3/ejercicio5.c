/**
 *@file ejercicio5.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@date: 04/04/2017
 *Grupo:2201 Pareja 4
 *
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include "semaforos.h"

#define SEMKEY 75797
#define FILEKEY "/bin/cat"
#define N_SEMAFOROS 2
/**
 *@brief main: programa que crea tantos N_SEMAFOROS definido anteriormente, 
 *inicializandolos a 1
 *realiza las siguentes operaciones en orden:
 *	Up del primer semaforo
 *	Down del segundo semaforo
 *	Up de ambos semaforos
 *	Down de ambos semaforos
 *	Borrar todos los semaforos
 */
 
int main(){

	int semid;
	int comprob;//, *valsem;
	//unsigned short *array = NULL;
	int i;
	key_t key;
	union semun {
   		int val;
   		struct sem_id_ds *semstat;
   		unsigned short *array;
	} arg;
	int active[N_SEMAFOROS];
	
	/**
	 *@brief Creacion de los semaforos
	 *obtenemos un key para el semaforo
	 *llamamos a Crear_Semaforo
 	 */
	key = ftok(FILEKEY, SEMKEY);
	if(key == -1){
		fprintf(stderr, "error con key");
		exit(EXIT_FAILURE);
	}
	
	if((comprob = Crear_Semaforo(key, N_SEMAFOROS, &semid)) != 0){	/*!<creamos N_SEMAFOROS*/
		if(comprob == OK){								
			printf("\nEl semaforo ya estaba creado\n");

		}
		else{
			printf("\nError al crear el semaforo\n");
		}
		exit(EXIT_FAILURE);								/*!<si no hemos podido crear el semaforo salimos*/
	}
	printf("\nSemaforo Creado con id: %d\n", semid);
	
	
	/**
	 *@brief Inicializacion de los semaforos
	 *creamos el array donde se almacenaran los valores iniciales de todos los semaforos
	 *llamamos a Inicializar_Semaforo
	 *Si nos devuelve error, borramos el semaforo y salimos del programa
 	 */
	arg.array = (unsigned short *)malloc(sizeof(short)*N_SEMAFOROS); 	/*!<array con valores iniciales de los semaforos*/
	for(i = 0; i < N_SEMAFOROS; i++){
		arg.array[i] = 1;
	}
	if(Inicializar_Semaforo(semid, arg.array) == ERR){
		printf("\nError al inicializar el semaforo\n");
		if(Borrar_Semaforo(semid) == ERR){
			printf("\nCatastrophic error\n");
			abort();
		}
		exit(EXIT_FAILURE);
	}
	/**
	 *@brief Operaciones con los semaforos
	 *realiza:
	 *	up del primer semaforo
	 *	down del segundo
	 *	Up de ambos (con UpMultiple_Semaforo)
	 *	Down de ambos (con DownMultiple_Semaforo)
	 *e imprime los valores de los semaforos despues de cada operacion
 	 */
	
	semctl(semid, 0, GETALL, arg);	/*!<guarda en arg, los valores actuales de los semaforos*/
	printf("\nSemaforo Inicializado con valores: ");
	for(i = 0; i < N_SEMAFOROS; i++){
		printf("%d ", arg.array[i]);
	}
	
	if(Up_Semaforo(semid, 0, SEM_UNDO) == ERR){		/*!<Up del semaforo 0*/
		printf("\nError al hacer UP\n");
	}
	
	semctl(semid, 0, GETALL, arg);			/*!<guarda en arg, los valores actuales de los semaforos*/
	printf("\nValores semaforos despues de UP: ");
	for(i = 0; i < N_SEMAFOROS; i++){
		printf("%d ", arg.array[i]);
	}
	
	if(Down_Semaforo(semid, 1, SEM_UNDO) == ERR){	/*!<Down del semaforo 1*/
		printf("\nError al hacer un UP\n");
	}
	
	semctl(semid, 0, GETALL, arg);				/*!<guarda en arg, los valores actuales de los semaforos*/
	printf("\nValores semaforos despues de DOWN: ");
	for(i = 0; i < N_SEMAFOROS; i++){
		printf("%d ", arg.array[i]);
	}
	
	
	
	for(i = 0; i < N_SEMAFOROS; i++){	/*!<array con los semaforos sobre los que trabajaremos (todos)*/
		active[i] = i;
	}
	
	if(UpMultiple_Semaforo(semid, N_SEMAFOROS, SEM_UNDO, active) == ERR){	/*!<Up de todos los semaforos*/
			printf("\nError al hacer un MULTIPLE UP\n");
	}
	semctl(semid, 0, GETALL, arg);				/*!<array con los semaforos sobre los que trabajaremos (todos)*/
	printf("\nValores semaforos despues de MULTIPLE UP: ");
	for(i = 0; i < N_SEMAFOROS; i++){
		printf("%d ", arg.array[i]);
	}
	
	if(DownMultiple_Semaforo(semid, N_SEMAFOROS, SEM_UNDO, active) == ERR){	/*!<Down de todos los semaforos*/
		printf("\nError al hacer un MULTIPLE DOWN\n");
	}
	semctl(semid, 0, GETALL, arg);				/*!<array con los semaforos sobre los que trabajaremos (todos)*/
	printf("\nValores semaforos despues de MULTIPLE DOWN: ");
	for(i = 0; i < N_SEMAFOROS; i++){
		printf("%d ", arg.array[i]);
	}
	printf("\n");
		
	/**
	 *@brief Borrar los semaforos
	 *liberamos el array de valores de los semaforos dentro de arg
	 *llamamos a Borrar_Semaforo
	 *si falla esta llamada, salimos del programa de manera anormal
 	 */
	free(arg.array);
	if(Borrar_Semaforo(semid) == ERR){
			printf("\nCatastrophic error\n");
			abort();
		}
	return 1;
}