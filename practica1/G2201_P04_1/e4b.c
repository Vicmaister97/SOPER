/*a) Analiza el árbol de procesos vinculado al siguiente código: */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define NUM_PROC 3
/**
 *@file eb.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que crea una serie de procesos mediante llamadas a fork y analiza el arbol
 *de procesos que se generan. Imprime para cada hijo, su PID y el de su padre
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/  

    
 int main (void){
         int pid, wstatus;
         int i;
         for (i = 0; i < NUM_PROC; i++){
 			
 			if ((pid = fork()) < 0 )
 			{
 			printf("Error haciendo fork\n"); 
 			exit(EXIT_FAILURE);
 			}
 			
           	else if (pid ==  0) printf("HIJO  %d con padre %d\n", getpid(), getppid());
           	
           	else{
			printf ("PADRE %d \n", pid);
			char cmd[80];
			sprintf(cmd,"pstree -p 	%d", pid);
			system(cmd);
		}
         }
         wait(&wstatus);
         exit(EXIT_SUCCESS);
       } 


