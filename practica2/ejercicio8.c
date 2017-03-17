#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int hijo_pid;
int rootPadre;

void main(int argc, char **argv){

    int numproc, numvuel;
    int pid = 0;
    int i, j;

    void manejador_SIGUSR1();
    void manejador_SIGTERM();

    if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("signal");
        exit(EXIT_FAILURE);
    }


    numproc = atoi(argv[1]);
    numvuel = atoi(argv[2]);

    if (numproc <= 0 || numvuel <= 0 || argc != 3){
        printf("Error en los parámetros de entrada\n");
        exit(EXIT_FAILURE);
    }

    rootPadre = getpid(); /*Guardamos en la variable global el pid del proceso raiz*/

    for (i = 0; i < numproc; i++){ /*Bucle que se encarga de crear los procesos*/

        pid = fork();
            
        if (pid == -1){
            printf("Error al crear el proceso hijo\n"); 
            exit(EXIT_FAILURE);
        }
            
        else if (pid == 0){

            if (i == numproc - 1){ /*Si es el último hijo*/
                hijo_pid = rootPadre;   
                sleep(5);   
                kill (hijo_pid, SIGUSR1);
            }
        }

        else{
            hijo_pid = pid; /*De esta forma, podemos enviar desde un proceso señales a su proceso hijo*/
            break;
        }
    }

    for (j = 0; j < numvuel+1; j++){ /*Bucle que se encarga del paso de testigo circular*/
        /*Armamos las señales*/
        if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
            perror("signal");
            exit(EXIT_FAILURE);
        }
        if(signal (SIGTERM, manejador_SIGTERM)==SIG_ERR){
            perror("signal");
            exit(EXIT_FAILURE);
        }
        pause();
    }

    if(getpid() == rootPadre){
        kill(hijo_pid, SIGTERM);
        pause();
    }
    else{
        pause();
    }

}

void manejador_SIGUSR1(int sig){
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output, 128, "%d/%m/%y %H:%M:%S", tlocal);
    printf("Hola PID = %d, time = %s\n", getpid(), output);
    sleep(2);
    kill (hijo_pid, SIGUSR1);
}

void manejador_SIGTERM(int sig){
    if (getpid() != rootPadre){ /*Excepto el proceso padre, todos esperan 1 segundo y envian SIGTERM al hijo*/
        sleep(1);
    }
    printf("Muere PID = %d\n", getpid());
    kill (hijo_pid, SIGTERM);
    wait(NULL);
    exit(EXIT_SUCCESS);
}