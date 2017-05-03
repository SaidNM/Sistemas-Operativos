#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>

typedef struct{
	int valor;
	char cadena[50];
}parametro;

void hilo1(void* arg){
	parametro *param=(parametro *)arg;
	printf("\n Este es el hilo %d \t %s \n ",param->valor,param->cadena);
	pthread_exit((void*)&param->valor);
}

int main(int argc, char const *argv[]){
	int numhilos = atoi(argv[1]); //pasamos parametros desde terminal
	pthread_t idphilo[numhilos];
	parametro arre[numhilos];
	int* valor;

	for(int j=0;j<numhilos;j++){
		arre[j].valor=j;
		strcpy(arre[j].cadena,"Soy un hilito");
		//strcat(arre[j].cadena,sprintf("%d",i));
		pthread_create(&idphilo[j],NULL,(void *)hilo1,(void *)&arre[j]);
	}

	for(int j=0;j<numhilos;j++){
		pthread_join(idphilo[j],(void*)&valor);
		printf("\nLlego el hilito %d \n",*valor);
	}

	return 0;
}