#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 300

int main(void)
{
	uint8_t inputBuffer[BUFFER_SIZE];
	int32_t bytesRead, bytesWrote, returnCode, fd, fd_log,fd_sign;
	
    
    /* Se crea el archivo Log.txt. Si el error es -1 no se puede. */
    /* O_APPEND --> Si el archivo existe se suman los datos al final. */
    if ( (fd_log = open("Log.txt", O_CREAT | O_APPEND | O_WRONLY, 0666) ) == -1  )   
    {
        printf("Error al crear el archivo Log.txt: %d\n", returnCode);
        exit(1);
    }

    /* Se crea el archivo Sign.txt. Si el error es -1 no se puede */
    /* O_APPEND --> Si el archivo existe se suman los datos al final. */
    if ( (fd_sign = open("Sign.txt", O_CREAT | O_APPEND | O_WRONLY, 0666) ) == -1  )   
    {
        printf("Error al crear el archivo Sign.txt: %d\n", returnCode);
        exit(1);
    }
    


    /* Se crea la cola nombrada. Si el error es -1 la FIFO ya existe */
    /*Si el error es menos que -1, no se puede crear la cola nombrada*/
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
    {
        printf("Error al crear la cola nombrada: %d\n", returnCode);
        exit(1);
    }
    
    /* Abro la cola nombrada. Blocks until other process opens it */
	printf("Esperando escritores...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    {
        printf("Error al abrir al archivo de la cola nombrada: %d\n", fd);
        exit(1);
    }
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("Tengo un escritor de la cola nombrada\n");



    /* Se repite el loop cuando bytesRead sea mayor a cero */
    /* Desde el escritor de la cola nombrada cuando se manda EOF es cero */
	do
	{
        /* Leo datos del buffer local, si no llegan datos se queda bloqueado en el read */
		if ((bytesRead = read(fd, inputBuffer, BUFFER_SIZE)) == -1)
        {
			perror("read");
        }
        else
		{
			// Detecto si es un Dato			
			if(inputBuffer[0]=='D')
			{			
				// Se agrega para que print detecte el fin			
				inputBuffer[bytesRead] = '\0';					
				fprintf(stdout,"Reader: leí %d bytes: %s \n", bytesRead, inputBuffer);

       				// Escribo en el archivo Log.txt.
				inputBuffer[bytesRead] = '\n';
				if ((bytesWrote = write(fd_log, inputBuffer, bytesRead+1)) == -1)
        			{
					perror("write");
        			}
        			else
        				{
						fprintf(stdout,"Log.txt: escribí %d bytes \n", bytesWrote);
        				}
			}

			// Detecto si es una Señal			
			if(inputBuffer[0]=='S')
			{			
				// Se agrega '\0' para que print detecte el fin			
				inputBuffer[bytesRead] = '\0';					
				fprintf(stdout,"Reader: leí %d bytes: %s \n", bytesRead, inputBuffer);

       				// Escribo en el archivo Log.txt.
				inputBuffer[bytesRead] = '\n';
				if ((bytesWrote = write(fd_sign, inputBuffer, bytesRead+1)) == -1)
        			{
					perror("write");
        			}
        			else
        				{
						fprintf(stdout,"Sign.txt: escribí %d bytes \n", bytesWrote);
        				}
			}
		}
	}
	while (bytesRead > 0);

	// Cierro los descriptores        
	close(fd_log);
	close(fd_sign);
	close(fd);

	return 0;
}
