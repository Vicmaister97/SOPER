/**
 *@file ejercicio6.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@date: 04/04/2017
 *Grupo:2201 Pareja 4
 *
*/ 


#include <stdio.h>
#include <stdlib.h>	
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/shm.h>
#include <signal.h>
#include "semaforos.h"


#define FILEKEY "/bin/cat"
#define KEY 1300
#define SEMKEY 75797
#define N_SEMAFOROS 3
#define CRITICAL 0			/*!<Semaforo para la seccion critica (mutex)*/
#define NOT_SO_CRITICAL 1	/*!<Semaforo 2*/
#define WHALE 2				/*!<Semaforo 3*/
#define ALFB 26				/*!<Numero de letras del abecedario ingles*/

/**
 *@brief letra nuestra estructura para la memoria compartida
 *tiene solo un char letra
 */
typedef struct _Letra letra;
struct _Letra{
	char letra;
};

/**
 *@brief main: programa productor-consumidor
 *el proceso padre creara un hijo que sera el productor de letras una a una de A,B...Z
 *el proceso hijo las escribira en memoria compartida y el proceso padre
 *las imprimira por pantalla, leyendolas una a una de la memoria compartida
 *se implementa con 3 semaforos para asegurar su correcto funcionamiento
 */
int main(int argc, char **argv){

	int numproc;
	int i;
	letra *buffer;
	int keymem, keysem, id_zone;
	int pid;
	int semid;
	int comprob;
	char letra_temp;
	char temp[ALFB] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	union semun {
   		int val;
   		struct sem_id_ds *semstat;
   		unsigned short *array;
	} arg;
	
	
	keymem = ftok(FILEKEY, KEY);	/*!<generamos un key para la memoria compartida*/
	if(keymem == -1){
		fprintf(stderr, "error con keymem");
		exit(EXIT_FAILURE);
	}

	keysem = ftok(FILEKEY, SEMKEY);	/*!<generamos un key para los semaforos */
	if(keysem == -1){
		fprintf(stderr, "error con keysem");
		exit(EXIT_FAILURE);
	}
	/**
 	 *@brief Creacion e inicializacion de los 3 semaforos
 	 *inicializamos CRITICAL y NOT_SO_CRITIAL a 1, y WHALE a 0
 	 */
	if((comprob = Crear_Semaforo(keysem, N_SEMAFOROS, &semid)) != 0){
		if(comprob == OK){
			printf("\nEl semaforo ya estaba creado\n");

		}
		else{
			printf("\nError al crear el semaforo\n");
		}
		exit(EXIT_FAILURE);
	}
	printf("\nSemaforo Creado con id: %d\n", semid);
	
	arg.array = (unsigned short *)malloc(sizeof(short)*N_SEMAFOROS); /*!<Array para valores iniciales de los semaforos*/
	for(i = 0; i < N_SEMAFOROS - 1; i++){
		arg.array[i] = 1; 					/*!<Inicializamos todos los semaforos a 1 menos el ultimo*/
	}

	arg.array[WHALE] = 0; 					/*!<WHALE se inicializa a 0 para evitar interbloqueo*/
	

	if(Inicializar_Semaforo(semid, arg.array) == ERR){
		printf("\nError al inicializar el semaforo\n");
		if(Borrar_Semaforo(semid) == ERR){
			printf("\nCatastrophic error\n");
			abort();
		}
		exit(EXIT_FAILURE);
	}


	/**
 	 *@brief Creacion e identificaion de la memoria compartida
 	 */
	id_zone = shmget(keymem, sizeof(letra), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	if(id_zone == -1){
		fprintf(stderr, "error creando memoria compartida");
		exit(EXIT_FAILURE);
	}
	
	buffer = shmat(id_zone, (char *)0, 0);
	if(buffer == NULL){
		fprintf(stderr, "error en shmat");
		exit(EXIT_FAILURE);
	}
	printf("\nMemoria compartida creada con id: %d\n", id_zone);
	
	printf("\nComenzando Productor-Consumidor\n");
	i = 0;
 	if ((pid = fork()) == -1){
 		printf("Error haciendo fork\n"); 
 		exit(EXIT_FAILURE);
 	}
 	/**
 	 *@brief Proceso Hijo: sera el productor de letras de 1 en 1
 	 *escribe letras de 1 en 1 en memoria compartida
 	 *y no podra volver a escribir hasta que el padre haya 
 	 *leido de esta memoria (semaforo CRITICAL)
 	 */		
    else if (pid ==  0){
		
		while(1){

			letra_temp = temp[i];

			if (Down_Semaforo(semid, NOT_SO_CRITICAL, SEM_UNDO) == ERR){ /*Asegura que el consumidor solo lea a medida que se produce una nueva letra */
				printf("Error haciendo down %d\n", i); 
 				exit(EXIT_FAILURE);
			}
			
			if (i == ALFB){ 				/*!<Ya no producimos más letras */
				shmdt ((char *)buffer);		/*!<Desconectamos el proceso de la memoria compartida*/
				free(arg.array);
				exit(EXIT_SUCCESS);
			}

			if (Down_Semaforo(semid, CRITICAL, SEM_UNDO) == ERR){ /*!<Asegura una escritura protegida */
				printf("Error haciendo down %d\n", i); 
 				exit(EXIT_FAILURE);
			}


			buffer->letra = letra_temp;

			if (Up_Semaforo(semid, CRITICAL, SEM_UNDO) == ERR){ /*!<Permite que el consumidor lea la letra escrita */
				printf("Error haciendo up %d\n", i); 
 				exit(EXIT_FAILURE);
			}

			if (Up_Semaforo(semid, WHALE, SEM_UNDO) == ERR){ /*!< Asegura al padre que el productor ha escrito en la memoria compartida */
				printf("Error haciendo up %d\n", i); 
 				exit(EXIT_FAILURE);
			}

			i++;
		}
		
    }
	 /**
 	 *@brief Proceso Padre: sera el consumidor de letras de 1 en 1
 	 *lee letras de 1 en 1 de memoria compartida
 	 *y no podra volver a leer hasta que el hijo haya 
 	 *escrito en esta memoria (semaforo CRITICAL)
 	 *Imprimira la letra que haya leido
 	 *cuando lea la 'Z', esperara a que el hijo acabe y terminara
 	 */	
    else{

    	while(1){

    		if (Down_Semaforo(semid, WHALE, SEM_UNDO) == ERR){ /*!<Asegura  que hay una nueva letra a leer */
				printf("Error haciendo down %d\n", i); 
 				exit(EXIT_FAILURE);
			}

			if (Down_Semaforo(semid, CRITICAL, SEM_UNDO) == ERR){ /*!<Asegura una escritura protegida */
				printf("Error haciendo down %d\n", i); 
 				exit(EXIT_FAILURE);
			}

			letra_temp = buffer->letra;

			if (Up_Semaforo(semid, CRITICAL, SEM_UNDO) == ERR){ /*!<Permite que el consumidor lea la letra escrita */
				printf("Error haciendo up %d\n", i); 
 				exit(EXIT_FAILURE);
			}

			if (Up_Semaforo(semid, NOT_SO_CRITICAL, SEM_UNDO) == ERR){ /*!<Asegura al padre que el productor ha escrito en la memoria compartida */
				printf("Error haciendo up %d\n", i); 
 				exit(EXIT_FAILURE);
			}
			//usleep(100000);
			printf("%c\n", letra_temp);
			if (letra_temp == 'Z'){ 			/*!<Ha leido la última letra */
				break;
			}

    	}

    	wait(NULL);							/*!<Espera a la terminacion del hijo*/

    }
	printf("\nFin Productor-consumidor\n");
    free(arg.array);

	if (Borrar_Semaforo(semid) == ERR){			/*!<Borramos el semaforo*/
		printf("\nCatastrophic error\n");
		abort();
	}
    
   
    shmdt ((char *)buffer); 		/*!<Desconectamos de la memoria compartida*/
    shmctl(id_zone, IPC_RMID, (struct shmid_ds *)NULL);	/*!<Borramos la memoria compartida*/
	
	exit(EXIT_SUCCESS);
}