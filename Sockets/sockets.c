#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *pull_conexion(void * arg){
	int*canal_id=(int*)arg;
	char buffer[50];
	int bytesRecibidos;
	while(1){
	//paso 4 Zona de comunicacion
		strcpy(buffer,"Hola bebe desde el servidor");
	//enviamos el mensaje al cliente
		if((send(*canal_id,(void *)&buffer,strlen(buffer),0))==-1){
			perror("\nSend error");
			exit(1);
		}

	//recibimos el mensaje del servidor

		if((bytesRecibidos=(recv(*canal_id,(void *)&buffer,sizeof(buffer),0)))==-1){ //recibimos el tamaño del arreglo de caracterres para evitar recibir 0 bytes
			perror("\nSend error");
			exit(1);
		}
		buffer[bytesRecibidos]='\0';
		printf("\n%s",buffer);
		if(strcmp(buffer,"fin")){
			break;
		}
	}
	//cerrando el canal
	close(*canal_id);

	//fin comunicacion
	free(canal_id);
}

int main(int argc, char const *argv[]){	
	int puerto = atoi(argv[1]);
	int sockid;
	int *canal_id;
	int tam;
	int bytesRecibidos; 
	char buffer[50];
	struct sockaddr_in servidor;
	struct sockaddr_in cliente;
	pthread_t *idhilo;


	//Creacion del socket 
	if((sockid=(socket(AF_INET,SOCK_STREAM,0)))==-1){
		perror("\nsocket");
		exit(1);
	}
	//paso 1 hacer publicable el servicio
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(puerto);
	servidor.sin_addr.s_addr = INADDR_ANY;
	if((bind(sockid,(struct sockaddr*)&servidor,sizeof(servidor)))==-1){
		perror("\nbind");
		exit(1);
	}
	//paso 2 hacer la solicitud de conexion
	if((listen(sockid,4))==-1){
		perror("\nListen error");
	}
	//paso 3 Aceptar la conexion
	while(1){
		idhilo=(pthread_t*)malloc(sizeof(pthread_t));
		canal_id=(int*)malloc(sizeof(int));
	tam=sizeof(cliente);
	if((*canal_id=accept(sockid,(struct sockaddr *)&cliente,&tam))==-1){
		perror("\nAccept error");
		exit(1);
		}
		pthread_create(idhilo,NULL,pull_conexion,(void*)canal_id);
		free(idhilo);
	}


	//---Construccion del cliente


	return 0;
}

