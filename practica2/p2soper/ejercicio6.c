#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

/**
 *@file ejercicio6.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief programa que crea un proceso hijo que imprime un mensaje con su pid cada 5 segundos. A los 30 segundos, el padre manda una 
 * señal de terminacion al hijo y termina.
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 

void main(){

    int pid;

    pid = fork();

    if (pid == -1){
        printf("Error al crear el proceso hijo\n");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0){ /*!<Proceso hijo*/

        while(1){
            printf("Soy el proceso hijo con PID: %d\n", getpid());
            sleep(5);
        }
    }

    else{ /*!<Proceso padre*/
        sleep(30);

        if (kill(pid, 9) == -1){
            printf("Error al enviar la señal de terminación al hijo\n");
        }

        printf("Señal de terminación enviada al hijo\n");

        exit(EXIT_SUCCESS);
    }

}
