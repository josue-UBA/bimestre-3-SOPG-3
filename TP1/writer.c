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

#include <signal.h> //para las señales
int32_t aux_fd;
int aux = 1;

void funcion_senal();
void recibiSignal(int sig);

void recibiSignal(int sig){
    printf("la señal es %d\n", sig);
    aux = 1;
    char outputBuffer[] = "señal 1";
    uint32_t bytesWrote;
    switch (sig){
        case 10: strcpy(outputBuffer, "SIGN:1\n"); break;
        case 12: strcpy(outputBuffer, "SIGN:2\n"); break;
        default: break;
    }
    if ((bytesWrote = write(aux_fd, outputBuffer, strlen(outputBuffer) - 1)) == -1){
        perror("error durante escritura");
    }
    else{
        printf("se escribieron %d bytes\n", bytesWrote);
    }
}

void funcion_senal(){
    struct sigaction sa;
    sa.sa_handler = recibiSignal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}

int main(void)
{
   
    char outputBuffer[BUFFER_SIZE];
	uint32_t bytesWrote;
	int32_t returnCode, fd;
    char intro[] = "intro ";

    /* Create named fifo. -1 means already exists so no action if already exists */
    if ( (returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0) ) < -1 )
    {
        printf("Error creating named fifo: %d\n", returnCode);
        exit(1);
    }

    /* Open named fifo. Blocks until other process opens it */
	printf("waiting for readers...\n");
	if ( (fd = open(FIFO_NAME, O_WRONLY) ) < 0 )
    {
        printf("Error opening named fifo file: %d\n", fd);
        exit(1);
    }
    aux_fd = fd;
    funcion_senal();
    
    /* open syscalls returned without error -> other process attached to named fifo */
	printf("got a reader--type some stuff\n");

    /* Loop forever */
	while (1)
	{
        /* Get some text from console */
        char mitemp[] = "DATA:";
		fgets(outputBuffer, BUFFER_SIZE, stdin);
        strcat(mitemp,outputBuffer);
        if(aux == 0){
        
        /* Write buffer to named fifo. Strlen - 1 to avoid sending \n char */
		if ((bytesWrote = write(fd, mitemp, strlen(mitemp)-1)) == -1)
        {
			perror("write");
        }
        else
        {
			printf("writer: wrote %d bytes\n", bytesWrote);
        }
        }
        aux=0;
	}
	return 0;
}
