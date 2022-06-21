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
#define TAM_BUFFER 300

int main(void)
{

    /* ************************** ABRIENDO ARCHIVOS */
    FILE *archivo_sign = fopen("../salidas/Sign.txt", "w");
    FILE *archivo_log  = fopen("../salidas/Log.txt", "w");
    /* ************************** */

	uint8_t inputBuffer[TAM_BUFFER];
	int32_t bytesRead, returnCode, fd;
    
    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1  )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }
    
    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for writers...\n");
	if ( (fd = open(FIFO_NAME, O_RDONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a writer\n");

    /* Loop until read syscall returns a value <= 0 */
	do
	{
        /* read data into local buffer */
		if ((bytesRead = read(fd, inputBuffer, TAM_BUFFER)) == -1)
        {
			perror("read");
        }
        else
		{
			inputBuffer[bytesRead] = '\0';
			printf("reader: read %d bytes: \"%s\"\n", bytesRead, inputBuffer);
            if(inputBuffer[0]=='S'){
                fprintf(archivo_sign, "%s\n", inputBuffer);
            }
            else{
                fprintf(archivo_log, "%s\n", inputBuffer);
            }
		}
	}
	while (bytesRead > 0);

    /* ************************** CERRANDO ARCHIVOS */
    fclose(archivo_sign);
    fclose(archivo_log);
    /* ************************** */

	return 0;
}
