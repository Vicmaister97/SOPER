/**
 *@file ejercicio2.c
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
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <signal.h>



#define FILEKEY "/bin/cat"
#define KEY 1300
#define MAX 100

/**
 *@brief info sera la estructura que utilizaremos en la memoria compartida
 *contiene un array nombre de tamano 80
 *contiene un int id 
 */
typedef struct _Info info;
struct _Info{
	char nombre[80];
	int id;
};

void manejador_SIGUSR1();

/**
 *@brief main: programa que crea n procesos hijos en paralelo. Los hijos duermen un tiempo
 *aleatorio y despues escriben en una zona de memoria compartida un nombre solicitado por
 *el proceso y un id que incrementan dentro de esta zona. Tras hacer esto mandan una señal
 *SIGUSR1 a su proceso padre que se encarga de imprimir por pantalla la informacion en 
 *la zona de memoria compartida. EL padre espera a que acaben todos los hijos antes de
 *borrar esta zona de memoria compartida y terminar.
 *@param argc int, numero de argumentos
 *@param argv, se espera un entero que sera el numero de procesos hijos a crear 
 */

int main(int argc, char **argv){

	int numproc;
	char temp[80];
	int i;
	info *buffer;
	int key, id_zone;
	int pidhijo, pid, wstatus;
	
	if(argc != 2){
		printf("Se espera un entero como parametro de entrada\n");
		exit(EXIT_FAILURE);
	}
	numproc = atoi(argv[1]);
	if(numproc < 1){
		printf("Se espera un entero mayor o igual que 1 como parametro de entrada\n");
		exit(EXIT_FAILURE);
	}
	
	key = ftok(FILEKEY, KEY);	/*!<generamos un key para la memoria compartida*/
	if(key == -1){
		fprintf(stderr, "error con key");
		exit(EXIT_FAILURE);
	}

	id_zone = shmget(key, sizeof(info), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);	/*!<creamos la zona de memoria compartida con permisos de lectura y escritura*/
	if(id_zone == -1){
		fprintf(stderr, "error creando memoria compartida");
		exit(EXIT_FAILURE);
	}
	
	buffer = shmat(id_zone, (char *)0, 0);					/*!<Identificamos la zona de memoria compartida*/
	if(buffer == NULL){
		fprintf(stderr, "error en shmat");
		exit(EXIT_FAILURE);
	}
	buffer->id = 0;				/*!<inicializamos el valor de id a 0*/
	
	
	if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){		/*!<Armamos la senal SIG_USR1*/
		perror("signal");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < numproc; i++){		/*!<Creamos tantos procesos hijos como numproc*/
 			
 			if ((pid = fork()) == -1){
 				printf("Error haciendo fork\n"); 
 				exit(EXIT_FAILURE);
 			}
 			/**
			 *@brief el hijo solicita un nombre al usuario
			 *el nombre se escribe en la zona de memoria compartida
			 *se aumenta el ID en la memoria compartida de manera que cada
			 *nombre tenga un ID distinto
			 *cuando termina esto, le manda la senal SIG_USR1 al proceso padre
			 *y termina
 			*/
           	else if (pid ==  0){
				pidhijo = getpid();
           		usleep(pidhijo);
           		printf("\nPID: %d Introduzca un nombre: ", pidhijo);
           		fgets(temp, MAX, stdin);
           		strcpy(buffer->nombre,temp);			
           		buffer->id++;
           		kill(getppid(), SIGUSR1);
           		shmdt ((char *)buffer);				/*!<El proceso hijo se desconecta de la memoria compartida*/
           		exit(EXIT_SUCCESS);
           	}
 			/**
			 *@brief el proceso padre espera a recibir una senal
			 *al recibirla imprime lo que haya leido de la zona de memoria compartida
			 *finalmente espera a que termine el hijo
 			*/
           	else{
           		pause();
           		printf("\nNombre : %sIdentidicador: %d\n", buffer->nombre, buffer->id);
           		wait(&wstatus);
         	}
       }
    shmdt ((char *)buffer);									/*!<El proceso se desconecta de la memoria compartida*/
    shmctl(id_zone, IPC_RMID, (struct shmid_ds *)NULL);		/*!<El proceso se borra la memoria compartida*/
	
	exit(EXIT_SUCCESS);
	
}

/**
 *@brief maneja la senal SIG_USR1 y la arma de nuevo
 *@param sig, int que contiene el numero de la senal que se le pasa
 */
void manejador_SIGUSR1(int sig){
	if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){		/*!<Arma la senal de nuevo*/
		perror("signal");
		exit(EXIT_FAILURE);
	}
}