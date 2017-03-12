#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>		/*perfecto menos du que tiene un puto problema (solo -l, -v)... */

#define CMD_MAX 100
/**
 *@file e8.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que crea tantos procesos como programas pasados por entrada y los ejecuta
 *segun el ultimo parametro pasado por entrada
 *@param argv[0] el nombre del programa
 *@param argv[1]...argv[n-1] los programas a ejecutar
 *@param argv[n] -l, -lp, -v, -vp dependiendo de como se quiera ejecutar los programas
 *pasados por entrada
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
int main(int argc, char **argv){
	
	int pid, wstatus;
	char path[CMD_MAX];
	for(int i = 1; i < argc - 1; i++){
		if ((pid = fork()) < 0 )
 		{
 		printf("Error haciendo fork\n"); 
 		exit(EXIT_FAILURE);
 		}

		else if (pid ==  0){
			char *arg[] = {argv[i], NULL};
			printf("\nHIJO == %d\n", getpid());
			sprintf(path,"%s%s","/bin/", arg[0]); /*!<creamos y guardamos el path*/
			
			if(strcmp(argv[argc -1], "-l") == 0){
				execl(path, arg[0], NULL);
				perror("\nOh dear, error...");
			}

			else if(strcmp(argv[argc -1], "-lp") == 0){
				execlp(arg[0], arg[0], NULL);
				perror("\nOh dear, error...");
			}
	
			else if(strcmp(argv[argc -1], "-v") == 0){
				execv(path, arg);
				perror("\nOh dear, error...");
			}		

			else if(strcmp(argv[argc -1], "-vp") == 0){
				execvp(arg[0], arg);
				perror("\nOh dear, error...");
			}
			else{	/*!<no hay flag que corresponda con un exec*/
				printf("\nERROR: Flag doesn't match any programs\n");
				exit(EXIT_SUCCESS);
			}
			
			exit(EXIT_FAILURE);
           	}

		else{	wait(&wstatus);
		
		}



	}
	
	exit(EXIT_SUCCESS);
}



