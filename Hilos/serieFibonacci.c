#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

void hilo1(void* arg){
	int *limite=(int*)arg;
	int serie;
	int aux1=0;
	int aux2=1;
	int resultado[*]
	for(int j=0;j<= *limite;j++){
		if(serie<*limite){
		serie=aux1+aux2;
		aux1=aux2;
		aux2=*serie;
		}
		else{
			break;
		}
	}

	pthread_exit((void*)&serie);
}

int main(int argc, char const *argv[]){
	int numhilos=0;
	printf("Serie de Fibonacci");
	printf(" \nNumero de hilos: ");
	scanf("%d",&numhilos);

	pthread_t idphilo[numhilos];
	int arre[numhilos];
	int *valor;

	for(int j=0;j<numhilos;j++){
		int r=rand()%100;
		srand(time(NULL));
		arre[j]=r;
		//strcpy(arre[j].cadena,"Soy un hilito");
		//strcat(arre[j].cadena,sprintf("%d",i));
		pthread_create(&idphilo[j],NULL,(void *)hilo1,(void *)&arre[j]);
	}

	for(int j=0;j<numhilos;j++){
		pthread_join(idphilo[j],(void*)&valor);
		printf("\nLlego el hilito %d y genero %d\n",valor);
	}

	return 0;
}