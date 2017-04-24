#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

void hilo1(void* arg){
	int *param=(int *)arg;
	printf("\n Este es el hilo %d \n ",*param);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[]){
	pthread_t idphilo[10];
	int valor[10];

	for(int j=0;j<10;j++){
		valor[j]=j;
		pthread_create(&idphilo[j],NULL,(void *)hilo1,(void *)&valor[j]);
	}
	for(int j=0;j<10;j++){
		pthread_join(idphilo[j],NULL);
	}

	return 0;
}