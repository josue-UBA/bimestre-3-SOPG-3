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


int main()
{
	socklen_t addr_len;
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	char buffer[128];


	// Creamos socket
	int s = socket(PF_INET,SOCK_DGRAM, 0);

	// Cargamos datos de IP:PORT del server
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(4096);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(serveraddr.sin_addr.s_addr==INADDR_NONE)
	{
		fprintf(stderr,"ERROR invalid server IP\r\n");
		return 1;
	}

	// Abrimos puerto con bind()
	if (bind(s, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
		close(s);
		perror("listener: bind");
	}

	// Escuchamos datagram con recvfrom()
	bzero((char *) &clientaddr, sizeof(clientaddr));
	addr_len = sizeof(clientaddr);

	int numBytes = recvfrom(s,buffer,127,0, (struct sockaddr*)&clientaddr, &addr_len );
	printf("Se recibieron %d bytes",numBytes);

	char* ipString = inet_ntoa(clientaddr.sin_addr);
	int port = ntohs(clientaddr.sin_port);
	printf(" desde ip:%s port:%d",ipString,port);

	buffer[numBytes]=0;
	printf("\nMSG:%s\n",buffer);

	// Envio respuesta
	sleep(2);
	printf("envio respuesta al cliente\r\n");
	sendto(s,"respuesta",10,0,(struct sockaddr*)&clientaddr,addr_len);

	sleep(2);
	printf("envio respuesta2 al cliente\r\n");
	sendto(s,"respuesta2",11,0,(struct sockaddr*)&clientaddr,addr_len);
	//___________________________________


	close(s);

	return 0;
}

