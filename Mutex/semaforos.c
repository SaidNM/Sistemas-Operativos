#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
int semid;
int zonaCritica[5];
struct sembuf operacion; //esta definida en sys/sem

void *waitS(int posicion){
	operacion.sem_num =posicion;
	operacion.sem_op =-1; //se realiza el decremento
	operacion.sem_flg = SEM_UNDO;
	if(semop(semid,&operacion,1) == -1){
		printf("\nError semop");
		exit(1);
	}
}
void *signalS(int posicion){
	operacion.sem_num =posicion;
	operacion.sem_op =1; //se realiza el incremento
	operacion.sem_flg = SEM_UNDO;
	if(semop(semid,&operacion,1) == -1){
		printf("\nError semop");
		exit(1);
	}
}


void *productor(void *arg){
	int cont=0;
	for(int i=0;i<5;i++){
		waitS(0);
		printf("productor %d : ",i);
		for(int j=0;j<5;j++){
			zonaCritica[j]=cont;
			printf("%d ",cont);
			cont++;
		}
		printf("\n");
		signalS(1);
	}
}
void *consumidor(void *arg){
	for(int i=0;i<5;i++){
		waitS(1);
		printf("consumidor %d : ",i);
		for(int j=0;j<5;j++){
			printf("%d ",zonaCritica[j]);
		}
		printf("\n");
		signalS(0);
	}
}
int main(int argc, char const *argv[])
{
	//Creacion de la llave
	key_t llave=ftok("/bin/ls",4); //Archivo que siempre exista --comando ls
	//Creacion del semaforo

	if(semid=(semget(llave,2, IPC_CREAT | IPC_EXCL | 0600))==-1){  //IPC_EXCL devuelve -1 si el semaforo ya esta creado
		exit(1);
	} //Devuelve menos -1 si no se creo el semaforo

	//Posicion 0 para productor
	//Posicion 1 para consumidor
	semctl(semid,0,SETVAL,1);
	semctl(semid,1,SETVAL,0);

	//Creacion de los hilos 
	pthread_t id_cons[3];
	pthread_t id_prod[3];

	for(int j=0;j<3;j++){  
		pthread_create(&id_cons[j],NULL,consumidor,NULL);
	}

	for(int j=0;j<3;j++){  
		pthread_create(&id_prod[j],NULL,productor,NULL);
	}


	//Esperamos a los hilos
	for(int j=0;j<3;j++){
		pthread_join(id_cons[j],NULL);
		pthread_join(id_prod[j],NULL);

	}



	return 0;
}