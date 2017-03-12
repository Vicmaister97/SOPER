#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 4
#define TAM 200
/**
 *@file e9.c
 *@autor victor garcia victor.garciacarrera@estudiante.uam.es, alfonso carvajal alfonso.carvajal@estudiante.uam.es
 *@brief Programa que crea 4 procesos hijos que en orden suman, restan, multiplican
 *y dividen 2 operandos que se pasan como parametro de entrada. Todos los hijos tienen un unico
 *padre que le pasa los parametros mediante una tuberia.
 *Cada hijo devuelve al padre el resultado de su operacion correspondiente.
 *El padre imprime el resultado devuelto por los hijos
 *@param argv[0] el nombre del programa
 *@param argv[2] x,y donde x e y son dos int
 *@date: 03/03/2017
 *Grupo:2201 Pareja 4
 *
*/ 
typedef enum{
	SUMA = 0,
	RESTA = 1,
	MULT = 2,
	DIV = 3
}OPERACION;
       
 int main (int argc, char **argv){
         int pipe1[2], pipe2[2];
	 int nbytes;
	 int pipe1_status, pipe2_status;
	 int  i, op1, op2, res;
	 pid_t pid, wstatus;
	 char readbuffer[TAM], sonbuff[TAM], buff[TAM], operacion[15];
	 char *c, *c1;
	 if(argc != 2){
		printf("\nSe esperan 2 enteros como parametro de entrada de la forma x,y\n");
		exit(EXIT_FAILURE);
	}
	 
	 pipe1_status = pipe(pipe1);			/*!<Creacion de los pipes */
	 pipe2_status = pipe(pipe2);
	 if(pipe1_status == -1 || pipe2_status == -1) {
		perror("Error creando la tuberia\n");
		exit(EXIT_FAILURE);
	}

         for (i = 0; i < NUM_PROC; i++){
 		
		if ((pid = fork()) < 0 )
 		{
 		printf("Error haciendo fork\n"); 
 		exit(EXIT_FAILURE);
 		}
 			
           	else if (pid ==  0){
			close(pipe1[1]);
			nbytes = read(pipe1[0], sonbuff, sizeof(sonbuff)); /*!<lee lo que le pasa el padre*/
			if(nbytes <= 0){
				printf("\nERROR de lectura\n");
				exit(EXIT_FAILURE);
			}
			c = strtok(sonbuff, ",");/*!<divide la cadena "x,y" en "x" e "y"*/
			c1 = strtok(NULL,sonbuff);
			op1 = atoi(c);
			op2 = atoi(c1);
			res = 0;
			if(i == SUMA){
				sprintf(operacion,"suma");
				res = op1 + op2;
			}
			else if(i == RESTA){
				sprintf(operacion,"resta");
				res = op1 - op2;
			}
			else if(i == MULT){
				sprintf(operacion,"multiplicacion");
				res = op1 * op2;
			}
			else if(i == DIV){
				sprintf(operacion,"division");
				res = op1/op2;
			}
			else{ printf("\nERROR\n");
				exit(EXIT_FAILURE);
			}
			close(pipe2[0]);
			sprintf(buff,"Datos enviados a través de la tubería por el proceso PID=%d. Operando 1: %d. Operando 2: %d. %s: %d", getpid(), op1, op2, operacion, res); /*!<guarda en un buffer la informacion a mandar al padre*/
			nbytes = write(pipe2[1], buff, strlen(buff) + 1);
			if(nbytes <=0){
				printf("\nERROR de escritura\n");
				exit(EXIT_FAILURE);
			}	
			exit(EXIT_SUCCESS);
           	}

           	else{
			write(pipe1[1], argv[1], strlen(argv[1]) + 1);
			wait(&wstatus);	/*!<escribe en el pipe y espera al hijo*/
			nbytes = read(pipe2[0], readbuffer, sizeof(readbuffer) + 1);
			if(nbytes <= 0){
				printf("\nERROR de lectura\n");
				exit(EXIT_FAILURE);
			}
			printf("\n%s\n",readbuffer);
		}
         }
         exit(EXIT_SUCCESS);
}

