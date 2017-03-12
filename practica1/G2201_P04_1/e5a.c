/*a) Analiza el árbol de procesos vinculado al siguiente código: */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 *@file e5b.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que crea una serie de procesos mediante llamadas a fork de manera que cada 
 *proceso solo tiene 1 hijo. Imprime para cada hijo, su PID y el de su padre.
 *Imprime para cada padre su pid
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
#define NUM_PROC 3

 int main (void){
         int pid, wstatus, pidhijo;
         int i;
         for (i = 0; i < NUM_PROC; i++){
 			
 			if ((pid = fork()) < 0 )
 			{
 			printf("Error haciendo fork\n"); 
 			exit(EXIT_FAILURE);
 			}
 			
           	else if (pid ==  0){
			pidhijo = getpid();
			printf("HIJO  %d con padre %d\n", pidhijo, getppid());
           	}

           	else{
			printf ("PADRE %d \n", getpid());
			wait(&wstatus);
			printf("\nThis process ID == %d is ending\n", getpid());
			exit(EXIT_SUCCESS);
		}
         }
	printf("\nThis process IDS == %d is ending\n", getpid());
         exit(EXIT_SUCCESS);
       }

