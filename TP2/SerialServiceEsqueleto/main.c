#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
// LIBRERIAS AGREGADAS
#include <pthread.h>
#include <signal.h>
#include "SerialManager.h"
// MACROS
#define PUERTO_DEL_SERV 10000
#define BUFFER_SIZE 128
#define MICRO_SLEEP 3000
#define CHART_UART '>'
// VARIABLES GLOBALES
static int newfd;
static int s;
// DECLARACION DE FUNCIONES
void *hiloParaUart(void *_);
static void senalHandler(int signal);
void bloquearSign(void);
void desbloquearSign(void);
//DEFINICIONES
void *hiloParaUart(void *_)
{
    static const char MSG_LEN = 7, EMPTY_CHAR = 32;
    static char uartbuff[BUFFER_SIZE] = {EMPTY_CHAR};
    for (;;)
    {
        /* esta funcion NO es bloqueante, por si acaso */
        if (serial_receive(uartbuff, BUFFER_SIZE))
        {
            printf("despues\n");
            if (*uartbuff != CHART_UART)
            {
                perror("Ignorning invalid msg");
                continue;
            }
            if (write(newfd, uartbuff, MSG_LEN) == -1)
            {
                perror("Error escribiendo mensaje en socket");
                exit(1);
            }
        }
        usleep(MICRO_SLEEP);
    }
}
static void senalHandler(int signal)
{
    close(newfd);
    close(s);
    serial_close();
    exit(0);
}
void bloquearSign(void)
{
    sigset_t set;
    int s;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    int ret = pthread_sigmask(SIG_BLOCK, &set, NULL);
    /* gestion de errores */

    if (ret < 0)
    {
        perror("pthread error");
        exit(1);
    }
}
void desbloquearSign(void)
{
    sigset_t set;
    int s;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    int ret = pthread_sigmask(SIG_UNBLOCK, &set, NULL);
    /* gestion de errores */

    if (ret < 0)
    {
        perror("pthread error");
        exit(1);
    }
}
// MAIN
int main()
{
    socklen_t addr_len;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    pthread_t t1;
    char buffer[BUFFER_SIZE];
    int n;
    struct sigaction sa;
    sa.sa_handler = senalHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    // Init UART
    serial_open(1, 115200);

    //Creamos socket y verificamos error
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s==-1){
        perror("socket error");
        exit(1);
    }

    // Cargamos datos de IP:PORT del server
    bzero((char*) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PUERTO_DEL_SERV);
    //serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(inet_pton(AF_INET, "127.0.0.1", &(serveraddr.sin_addr))<=0)
    {
        fprintf(stderr,"ERROR invalid server IP\r\n");
        return 1;
    }

    // Abrimos puerto con bind()
    if (bind(s, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        close(s);
        perror("listener: bind");
        return 1;
    }

	// Seteamos socket en modo Listening
	if (listen (s, 10) == -1) // backlog=10
  	{
    	    perror("error en listen");
    		exit(1);
  	}

    while(1)
    {
        // Ejecutamos accept() para recibir conexiones entrantes
        addr_len = sizeof(struct sockaddr_in);
        if ( (newfd = accept(s, (struct sockaddr *)&clientaddr,&addr_len)) == -1)
        {
            perror("error en accept");
            exit(1);
        }

        char ipClient[32];
        inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient));
        printf  ("server:  conexion desde:  %s\n",ipClient);

        /* bloquear antes de crear el thread para que herede el bloqueo */
        bloquearSign();
        /* se crea hilo para la comunicacion con la EDUCIA */
        int ret = pthread_create(&t1, NULL, hiloParaUart, NULL);
        /* desbloquear para que el thread principal interprete las señales */
        desbloquearSign();
        /* gestion de errores */
        if (ret)
        {
            errno = ret;
            perror("pthread error");
            exit(1);
        }
        /*  
        - este while itera constantemente. No sale.
        - la funcion read() es bloqueante 
        - el valor retornado por la funcion read() es el numero de bytes
        */
        printf("antes - servidor\n");
        while ((n = read(newfd, buffer, BUFFER_SIZE)) > 0)
        {
            buffer[n] = '\0';
            /* serial_send envia el dato recibido de la pagina web a la EDUCIA */
            serial_send(buffer, n);
            /* se ejecuta cuando se recibe informacion de la web */
            printf("Numero de bytes %d. String recibido:%s\n", n, buffer);
        }
        /* gestion de errores */
        if( (n = read(newfd,buffer,BUFFER_SIZE)) == -1 )
        {
            perror("Error leyendo mensaje en socket");
            exit(1);
        }
        printf("Se cerro la conexion. Se sale del while.\n");
    } // fin while
    // Cerramos conexion con cliente
    close(newfd);
    close(s);
    serial_close();

    return 0;
}