#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

// Defino Constantes
#define FIFO_NAME 	"myfifo"
#define MSG_DATA 	"DATA:"
#define MSG1 		"SIGN:1"
#define MSG2 		"SIGN:2"
#define SIZE_MSG 	6
#define BUFFER_SIZE 300

// Variables globales
char outputBuffer[BUFFER_SIZE];
uint32_t bytesWrote;
int32_t returnCode, fd;


// Señales

void Signal_receive(int signal)
{
    
    write(1,"Capturo las señales...\n",23);
   
    if (signal==SIGUSR1)
    {
        strcpy(outputBuffer, MSG1);
    }

    if (signal==SIGUSR2)
    {
       strcpy(outputBuffer, MSG2);
    }

    /* Escribo en el buffer de la cola nombrada. Strlen - 1 to avoid sending \n char */  
    if ((bytesWrote = write(fd, outputBuffer, SIZE_MSG)) == -1)
    {
    	perror("write");
    }
    else
    	{
    	    printf("Writer: escribí %d bytes\n", bytesWrote);
        }
}





//Programa principal

int main(void)
{
    char temp[BUFFER_SIZE];    
    
   
    /* Se crea la cola nombrada. Si el error es -1 la FIFO ya existe */
    /*Si el error es menos que -1, no se puede crear la cola nombrada*/
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error al crear la cola nombrada: %d\n", returnCode);
        exit(1);
    }

    /* Abro la cola nombrada como lectura */
	printf("Esperando que se conecten lectores...\n");
	/* Si no hay lectores se queda bloqueado en open, lo bloque ael S.O. */
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error al abrir al archivo de la cola nombrada: %d\n", fd);
        exit(1);
    }
    
    /* El syscalls open retorna */
	printf("Se posee un lector de la cola nombrada, se puede escribier algo desde la consola\n");

    
    /* Configuro las señales */
       struct sigaction sa;
       sa.sa_handler = Signal_receive;
       sa.sa_flags = SA_RESTART; //SA_RESTART;
       sigemptyset(&sa.sa_mask);
       if(sigaction(SIGUSR1,&sa,NULL)==-1){
		perror("Error de sigaction: SIGUSR1");
        	exit(1);
        }
       if(sigaction(SIGUSR2,&sa,NULL)==-1){
		perror("Error de sigaction: SIGUSR2");
        	exit(1);
        }



    /* Loop forever */
	while (1)
	{
        	/* Espera texto desde la consola */
		fgets(outputBuffer, BUFFER_SIZE, stdin);

                if(strlen(outputBuffer) > 1 ) {                  // Si se presiona solamente ENTER no manda nada
			strcpy(temp,MSG_DATA);
                	strcat(temp,outputBuffer);
         		/* Escribo en el buffer de la cola nombrada. Strlen - 1 to avoid sending \n char */
               	 	if ((bytesWrote = write(fd, temp, strlen(temp)-1)) == -1)
        		{
				perror("write");
        		}
        		else
        			{
					printf("Writer: escribí %d bytes\n", bytesWrote);
        			}
                }
	}
	return 0;
}
