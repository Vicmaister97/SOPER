#include <stdio.h>
#include <stdlib.h>	
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <signal.h>


#define FILEKEY "/bin/cat"
#define KEY 1300
#define MAX 100


typedef struct _Info info;
struct _Info{
	char nombre[80];
	int id;
};

void manejador_SIGUSR1();

int main(int argc, char **argv){

	int numproc;
	char temp[80];
	int i;
	info *buffer;
	int key, id_zone;
	int pidhijo, pid, wstatus;
	if(argc != 2){
		printf("Error en los parámetros de entrada\n");
		exit(EXIT_FAILURE);
	}
	numproc = atoi(argv[1]);
	
	key = ftok(FILEKEY, KEY);
	if(key == -1){
		fprintf(stderr, "error con key");
		exit(EXIT_FAILURE);
	}
	/**Creando memoria compartida*/
	id_zone = shmget(key, sizeof(info), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	if(id_zone == -1){
		fprintf(stderr, "error creando memoria compartida");
		exit(EXIT_FAILURE);
	}
	
	buffer = shmat(id_zone, (char *)0, 0);					//AQUI POR SI ERROR MIRAR
	if(buffer == NULL){
		fprintf(stderr, "error en shmat");
		exit(EXIT_FAILURE);
	}
	buffer->id = 0;
	
	
	if(signal(SIGUSR1, manejador_SIGUSR1) == SIG_ERR){		//ARMANDO LA SEÑAL
		perror("signal");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < numproc; i++){
 			
 			if ((pid = fork()) == -1){
 				printf("Error haciendo fork\n"); 
 				exit(EXIT_FAILURE);
 			}
 			
           	else if (pid ==  0){
				pidhijo = getpid();
           		usleep(pidhijo);
           		/*PONER MASCARA EN ESTA PARTE*/
           		printf("\nPID: %d Introduzca un nombre: ", pidhijo); //Quitar el PID
           		fgets(temp, MAX, stdin);
           		//scanf(temp);
           		//gets(temp);
           		strcpy(buffer->nombre,temp);									//TRIPLAZO per hacemos asignacion dsd hijo
           		buffer->id++;
           		kill(getppid(), SIGUSR1);
           		shmdt ((char *)buffer);									//NPI de esto
           		exit(EXIT_SUCCESS);
           	}

           	else{
           		pause();
           		printf("\nNombre : %sIdentidicador: %d\n", buffer->nombre, buffer->id);
           		wait(&wstatus);
         	}
       }
    shmdt ((char *)buffer);									//NPI de esto
    shmctl(id_zone, IPC_RMID, (struct shmid_ds *)NULL);
	
	exit(EXIT_SUCCESS);
	
}

void manejador_SIGUSR1(int sig){
	if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("signal");
        exit(EXIT_FAILURE);
    }

}