#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define WORDS 13

int aleat_num(int inf, int sup){ /*Función que genera números aleatorios entre dos valores dados*/
    return inf + rand() % (sup - inf + 1);
}

void main(){

    int pid;
    char frase[] = "EL PROCESO A ESCRIBE EN UN FICHERO HASTA QUE LEE LA CADENA FIN";
    char *palabras[WORDS]; /*Array donde vamos a guardar las diferentes palabras de la frase*/
    int i;
    int activo;
    int random;
    char buf[100];
    int lecturas;
    int flag = 0;
    FILE *fp;

    srand(time(NULL));

    void manejador_SIGUSR1();

    palabras[0] = strtok(frase, " "); /*Guardamos en el array palabras las diferentes palabras de la frase(separadas por un espacio)*/

    for (i = 0; i < WORDS - 1; i++){
        palabras[i+1] = strtok(NULL, " ");
    }

    fclose(fopen("palabras.txt", "w"));

    lecturas = 0; /*Inicializamos el número de palabras leidas a 0*/

    pid = fork();

    while(1){
        if (pid < 0){
            printf("Error al crear el proceso hijo\n");
            exit(EXIT_FAILURE);
        }

        else if (pid == 0){ /*Proceso A*/
            srand(time(NULL));
            while(1){
                if (signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
                    perror("signal");
                    exit(EXIT_FAILURE);
                }

                random = aleat_num(0, WORDS - 1);
                fp = fopen("palabras.txt", "a");
                fprintf(fp, "%s\n", palabras[random]); /*Escribimos la palabra aleatoria en el fichero*/
                printf("Palabra escrita: %s\n", palabras[random]);
                fclose(fp);

                if (strcmp (palabras[random], "FIN") == 0){ /*Ha escrito la palabra FIN en el fichero*/
                    exit(EXIT_SUCCESS);
                }
            }
            
        }

        else if (pid > 0){ /*Proceso B*/
            
            flag = 0;
            sleep(5);
            activo = waitpid(pid, NULL, WNOHANG);

            if (activo == -1){
                perror("waitpid error");
                exit(EXIT_FAILURE);
            }

            else if (activo == 0){ /*El hijo está activo, ejecutandose*/
                kill (pid, SIGUSR1); /*De esta forma nos aseguramos que no haya problemas cuando B lea de fichero pues A está escribiendo en él, aunque es improbable que suceda*/
                activo = pid;
            }

            else if (activo == pid){ /*El hijo ha acabado*/
                fp = fopen("palabras.txt", "r");

                while(fscanf(fp, "%s", buf) != EOF){
                    lecturas ++;

                    if (strcmp(buf, "FIN") == 0){ /*Si lee la palabra FIN, cambia el flag a 1 para poder revivir al proceso A*/
                        flag = 1;
                        printf ("Palabras leidas = %d\n", lecturas);
                        break;
                    }

                    if(lecturas == 50){ /*Si el proceso B ha leido 50 cadenas, envia una señal de terminación a A y finaliza su ejecución*/
                        fclose(fp);
                        printf("Ha leido 50 cadenas\n");
                        kill(pid, SIGKILL);
                        return EXIT_SUCCESS;
                    }
                }
                fclose(fp);
            }

            fclose(fopen("palabras.txt", "w"));
            if(flag == 1){
                pid = fork();
            }
        }
    }
    
}

void manejador_SIGUSR1(int sig){

    if(signal (SIGUSR1, manejador_SIGUSR1)==SIG_ERR){
        perror("signal");
        exit(EXIT_FAILURE);
    }
    
}