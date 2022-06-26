/******************************
 * librerias
 ******************************/
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
#include <signal.h>
#include <pthread.h>
#include "SerialManager.h"
/******************************
 * macros
 ******************************/
#define PUERTO_DEL_SERV 10000
#define BUFFER_SIZE 128
#define MICRO_SLEEP 3000
#define CHART_UART '>'
/******************************
 * variables globales
 ******************************/
int newfd;
int n;
static pthread_t thing1;
/******************************
 * definicion de funciones
 ******************************/
void *start_thread(void *message);
void bloquearSign(void);
void desbloquearSign(void);
void sigint_handler(int sig);
/******************************
 * implementacion de funciones
 ******************************/
void *start_thread(void *message)
{
	int i;
	static const char MSG_LEN = 7, EMPTY_CHAR = 32;
	static char uartbuff[BUFFER_SIZE] = {EMPTY_CHAR};
	for (;;)
	{
		if (serial_receive(uartbuff, BUFFER_SIZE))
		{
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
		printf("en hilo\n\r");
		sleep(1);
	}
	return NULL;
}

void bloquearSign(void)
{
	sigset_t set;
	int s;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	// sigaddset(&set, SIGUSR1);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
}

void desbloquearSign(void)
{
	sigset_t set;
	int s;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	// sigaddset(&set, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
}

void sigint_handler(int sig)
{
	write(0, "Se sale ya que ingreso alguna de las señales SIGINT y SIGTERM\r\n", 80);
}
/******************************
 * funcion main
 ******************************/
int main()
{
	// Se establecen las señales a las que respondera el hilo principal: SIGINT y SIGTERM
	serial_open(1, 115200);
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}

	socklen_t addr_len;
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	char buffer[BUFFER_SIZE];

	printf("server - socket\n\r");
	// Creamos socket
	int s = socket(AF_INET, SOCK_STREAM, 0);

	// Cargamos datos de IP:PORT del server
	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PUERTO_DEL_SERV);
	// serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (inet_pton(AF_INET, "127.0.0.1", &(serveraddr.sin_addr)) <= 0)
	{
		fprintf(stderr, "ERROR invalid server IP\r\n");
		return 1;
	}

	printf("server - bind\n\r");
	// Abrimos puerto con bind()
	if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
	{
		close(s);
		perror("listener: bind");
		return 1;
	}

	printf("server - listen\n\r");
	// Seteamos socket en modo Listening
	if (listen(s, 10) == -1) // backlog=10
	{
		perror("error en listen");
		exit(1);
	}

	while (1)
	{
		// Ejecutamos accept() para recibir conexiones entrantes
		addr_len = sizeof(struct sockaddr_in);
		printf("server - accept\n\r");
		if ((newfd = accept(s, (struct sockaddr *)&clientaddr, &addr_len)) == -1)
		{
			perror("error en accept");
			exit(1);
		}

		char ipClient[32];
		inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient));
		printf("server:  conexion desde:  %s\n", ipClient);
		// se bloquea las señales antes de crear el hilo para que no le afecte las señales SIGINT SIGTERM
		printf("Bloqueo signal\n");
		bloquearSign();
		// se crea el hilo que gestionara la entrada de datos del hardware y el envio de datos al cliente
		const char *message1 = "Thing 1";
		pthread_create(&thing1, NULL, start_thread, (void *)message1);
		// Se desbloquea señal 
		printf("Desbloqueo signal\n");
		desbloquearSign();

		while (1)
		{
			printf("read\n\r");
			// si hay error se sale del programa
			if ((n = read(newfd, buffer, BUFFER_SIZE)) == -1)
			{
				perror("Error leyendo mensaje en socket");
				exit(1);
			}
			// si el cliente se desconecta, entonces se sale del bucle
			if (n == 0)
			{
				break;
			}
			// si no ocurre ninguna de las dos opciones anteriores, entonces se envia el dato recibido del cliente al hardware 
			buffer[n] = 0x00;
			printf("Recibi %d bytes.:%s\n", n, buffer);
			serial_send(buffer, n);
			printf("Numero de bytes %d. String recibido:%s\n", n, buffer);
		}
		// Cerramos conexion con cliente
		close(newfd);
		// se elimina el hilo creado
		pthread_cancel(thing1);
	}
	return 0;
}
