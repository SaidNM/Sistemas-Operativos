#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.>


int zonaCritica[10];
key_t ftok(const char* path, int projectID);
//pthread_mutex_t candado;

void *productor(void* arg){
	int aux=0;
	for(int i=0;i<5;i++){
		#ifdef MUTEX
		pthread_mutex_lock(&candado);
		printf("\nProduciendo... ciclo: %d",i);
		#endif
		for(int j=0;j<10;j++){
			zonaCritica[j]=aux;
			aux++;
		}
		//sleep(1);
		#ifdef MUTEX 
		pthread_mutex_unlock(&candado);
		#endif
	}
}
void *consumidor(void* arg){
	for(int i=0;i<5;i++){
		#ifdef MUTEX
		//sleep(1);
		pthread_mutex_lock(&candado);
		printf("\nProduciendo... ciclo: %d",i);
		#endif
		for(int j=0;j<10;j++){
			printf("\t%d",zonaCritica[j]);
		}
		#ifdef MUTEX
		pthread_mutex_unlock(&candado);
		#endif
		printf("\n");
	}	
}

int main(int argc, char const *argv[])
{
	pthread_t idhilo[2];
	pthread_mutex_init(&candado,NULL); //inicializa el candado

	for(int j=0;j<2;j++){  //Creacion de los hilos 
		if(j==0){ // Productor
			pthread_create(&idhilo[j],NULL,productor,NULL);
			//sleep(1);
		}
		else{	//Consumidor
			pthread_create(&idhilo[j],NULL,consumidor,NULL);
		}
	}

	for(int j=0;j<2;j++){
		#ifdef FIN
		pthread_join(idhilo[j],NULL);
		#endif 

	}

	return 0;
}