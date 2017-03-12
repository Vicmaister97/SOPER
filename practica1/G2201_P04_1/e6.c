#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define STR_SIZE 80
/**
 *@file e6.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que reserva memoria para un String y la rellena con la entrada recibida de
 *stdin desde un proceso hijo.
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
int main(void){

	char *buf = NULL;
	int pid, status = 0;
	buf = (char *)malloc(sizeof(char) * STR_SIZE);	
	if ((pid = fork()) < 0 )
 	{
 	printf("Error haciendo fork\n");	
	exit(EXIT_FAILURE);
	}

	else if(pid == 0){
		printf("\nIntroduce un nombre: ");
		fgets(buf, STR_SIZE, stdin);
		printf("\nHIJO === %d", getpid());
		printf("\nBUF == %s\n", buf);
		free(buf);
	}
	
	else{
		wait(&status);
		printf("\nPADRE == %d\n", getpid());
		//printf("\nBUF == %s\n", buf);
		free(buf); 

	}
	exit(EXIT_SUCCESS);
}

