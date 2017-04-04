#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/shm.h>
#include "semaforos.h"


int Inicializar_Semaforo(int semid, unsigned short *array){
	if (semid < 0 || array == NULL){
		return ERR;
	}

	union semun {
		int val;
		struct semid_ds *semstat;
		unsigned short *array;
	} arg;

	arg.array = array;

	if (semctl(semid, 0 , SETALL, arg) == -1){ /*Ponemos 0 de forma arbitraria, pues esta instrucción de control ignora el argumento semnum*/
		return ERR;
	}
	return OK;									
}

int Borrar_Semaforo(int semid){
	if (semid < 0){
		return OK; /*No hay semáforo que borrar*/
	}

	union semun {
		int val;
		struct semid_ds *semstat;
		unsigned short *array;
	} arg;

	if (semctl(semid, 0, IPC_RMID, 0) == -1){ /*Ponemos 0 de forma arbitraria, pues esta instrucción de control ignora el argumento semnum*/
		return ERR;
	}

	free(arg.array);
	return OK;
}

int Crear_Semaforo(key_t key, int size, int *semid){
	if (key < 0 || size < 1){
		return ERR;
	}

	unsigned short *array;
	int i;

	*semid = semget(key, size, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);

	if((*semid == -1) && errno == EEXIST){ /*Ya existía un semáforo asociado a la clave key*/
		return 1;
	}

	if(*semid == -1){
		return ERR;
	}

	array = (unsigned short *)malloc(sizeof(short)*size);
	if (array == NULL){
		return ERR;
	}

	for (i = 0; i < size; i++){
		array[i] = 0;
	}

	if (Inicializar_Semaforo(*semid, array) == ERR){
		return ERR;
	}

	return 0;
}

int Down_Semaforo(int semid, int num_sem, int undo){
	if (semid < 0 || num_sem < 0){
		return ERR;
	}

	struct sembuf sem_oper;

	sem_oper.sem_num = num_sem; /* Actuamos sobre el semáforo recibido como parámetro de entrada de la función */
	sem_oper.sem_op = -1; /* Decrementar en 1 el valor del semáforo pues es la función de Down */
	sem_oper.sem_flg = undo; /* Para evitar interbloqueos si un proceso acaba inesperadamente */
	if (semop(semid, &sem_oper, 1) == -1){
		return ERR;
	}

	return OK;
}

int DownMultiple_Semaforo(int semid, int size, int undo,int *active){
	if (semid < 0 || size < 1 || active == NULL){
		return ERR;
	}

	int i;

	for (i = 0; i < size; i++){
		if (Down_Semaforo(semid, active[i], undo) == ERR){
			return ERR;
		}
	}

	return OK;
}

int Up_Semaforo(int semid, int num_sem, int undo){
	if (semid < 0 || num_sem < 0){
		return ERR;
	}

	struct sembuf sem_oper;

	sem_oper.sem_num = num_sem; /* Actuamos sobre el semáforo recibido como parámetro de entrada de la función */
	sem_oper.sem_op = 1; /* Incrementar en 1 el valor del semáforo pues es la función de Up */
	sem_oper.sem_flg = undo; /* Para evitar interbloqueos si un proceso acaba inesperadamente */
	if (semop(semid, &sem_oper, 1) == -1){
		return ERR;
	}

	return OK;
}

int UpMultiple_Semaforo(int semid, int size, int undo, int *active){
	if (semid < 0 || size < 1 || active == NULL){
		return ERR;
	}

	int i;

	for (i = 0; i < size; i++){
		if (Up_Semaforo(semid, active[i], undo) == ERR){
			return ERR;
		}
	}

	return OK;
}