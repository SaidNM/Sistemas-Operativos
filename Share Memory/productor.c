#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define NUM_SEM 2
#define SEM_PRODUCTOR 0
#define SEM_CONSUMIDOR 1

int semid;
int shmid;

typedef struct{
	int flag;
	char compania;
	char operacion[50];
	char usuario[20];	
}Zona;

Zona *Memoria;

struct{
    unsigned short int sem_num;
    short int sem_op;
    short int sem_flg;
}sembuf;

union semun{
    int val;                /* value for SETVAL */
    struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
    ushort *array;          /* array for GETALL & SETALL */
    struct seminfo *__buf;  /* buffer for IPC_INFO */
    void *__pad;
};

void init_semaphore(int semnum,int initval){
    union semun semopts;
    semopts.val = initval;

	if(semctl(semid,semnum,SETVAL,semopts) == -1){
		perror("semctl");
		exit(1);
	}
}

void remove_semaphores(){
	union semun semopts;
	if (semctl(semid,0,IPC_RMID,semopts) == -1) {
        perror("semctl");
        exit(1);
    }
}

int CreaLigaMemoria(key_t key){ 

  //Verifica si el segmento de memoria existe (IPC_CREAT|IPC_EXCL)
  if((shmid = shmget(key,sizeof(Zona),IPC_CREAT|IPC_EXCL|0666)) == -1){
      /* El segmento existe - abrir como cliente */
      if((shmid = shmget(key,sizeof(Zona),0)) == -1){
	       perror("shmget");
	       exit(1);
      }
      else{
        printf("Yo soy el productor y me ligue a la memoria\n");
      }
  }
  else{
    printf("Yo soy el productor y cree la memoria\n");
  }

  //se ata a la zona de memoria creada
  if((Memoria = (Zona*)shmat(shmid,(Zona*) 0,0)) == (void *)-1) { 
      perror("shmat");
      exit(1);
  }
  //printf("%s\n",Memoria); //lee en memoria compartida

  return shmid; //regresa el identificador de la memoria
}

void DestruyeMemoriaCompartida(int id_Mem,Zona *buffer){
  //Terminada de usar la memoria compartida se libera
  if(id_Mem != 0 || id_Mem != -1){ //si la memoria ya fue destruida solo se des asociasa de ella
    shmdt (buffer);//des asocia la memoria compartida de la zona de datos de nuestro programa
    shmctl (id_Mem, IPC_RMID, (struct shmid_ds *)NULL);//destruye la zona de memoria compartida
    //printf("\nLa memoria compartida ha sido destruida OUT\n");
    printf("\nDestruyo la memoria\n");
  }
  else{
    shmdt (buffer);//des asocia la memoria compartida de la zona de datos de nuestro programa
    printf("\nSe Desligo de la memoria\n");
  }
}

int main(int argc, char const *argv[]){
	key_t key;
	pthread_t id_productor;


	/* create a unique key */
	if((key = ftok("/bin/ls",'K')) == -1){
		perror("ftok");
		exit(1);
	}

	//----------Crear e inicializar semaforos----------------------

	/* create a semaphore set with 2 semaphores */
	if((semid = semget(key,NUM_SEM,0666 | IPC_CREAT | IPC_EXCL)) == -1){
		if((semid = semget(key,2,0666))==-1){
			perror("semget");	
			exit(1);	
		}
		else{
			printf("Yo soy el productor y me ligue al semaforo\n");

		}
	}
	else{
		//Inicializacion de los semaforos
		init_semaphore(SEM_PRODUCTOR,1); //el primer semaforo se inicializo en 1
		init_semaphore(SEM_CONSUMIDOR,0); //el segundo semaforo se inicializo en 0
		printf("Yo soy el productor y cree e inicialize el semaforo\n");
	}

	//---------Crear e inicializar la memoria------------------
		shmid = CreaLigaMemoria(key);
		
	return 0;
}