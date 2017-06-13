#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *argv[]){	
	int puerto = atoi(argv[1]);
	int sockid;
	int canal_id;
	int tam;
	int bytesRecibidos; 
	char buffer[50];
	struct sockaddr_in servidor;

	//Creacion del socket 
	if((sockid=(socket(AF_INET,SOCK_STREAM,0)))==-1){
		perror("\nsocket");
		exit(1);
	}
	//paso 1 hacer conexion
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(puerto);
	servidor.sin_addr.s_addr = inet_addr(argv[2]);
	if((connect(sockid,(struct sockaddr*)&servidor,sizeof(servidor)))==-1){
		perror("\nconnect");
		exit(1);
	}

	while(1){
	//recibimos el mensaje del servidor

		if((bytesRecibidos=(recv(sockid,(void *)&buffer,sizeof(buffer),0)))==-1){ //recibimos el tamaño del arreglo de caracterres para evitar recibir 0 bytes
			perror("\nSend error");
			exit(1);
		}
		buffer[bytesRecibidos]='\0';
		printf("\nServidor: %s \n",buffer);
		printf("\nTeclea un mensaje\n");
		scanf("%s^[\0]",buffer);

		strcpy(buffer,"Soy cliente recibi mensaje");
		//respondemos al servidor
		if((send(sockid,(void *)&buffer,strlen(buffer),0))==-1){
			perror("\nSend error");
			exit(1);
		}
		if(strcmp(buffer,"fin")){
			break;
		}
	}

	//cerrando el canal
	close(sockid);


	return 0;
}

