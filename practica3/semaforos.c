#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include "semaforos.h"

int Inicializar_Semaforo(int semid, unsigned short *array){
	if (semid < 0 || array == NULL){
		return ERR;
	}

	
}