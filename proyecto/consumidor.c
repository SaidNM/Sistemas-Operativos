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
#include <string.h>
#define NUM_SEM 8
#define NUM_CONS 2
#define TAM_MEMORIA 4

//Variables Globales
int semid;
int shmid;
int vueltas,usuario1,usuario2,usuario3;

typedef struct{
	char compania;
	char mensajes[20];
	int usuario;
	char bandera;	
}ZonaMensajes;

ZonaMensajes *Memoria;

struct sembuf operacion;

// --Funciones
int CreaLigaMemoria(key_t key){ 

  //Verifica si el segmento de memoria existe (IPC_CREAT|IPC_EXCL)
  if((shmid = shmget(key,sizeof(ZonaMensajes)*TAM_MEMORIA,IPC_CREAT|IPC_EXCL|0666)) == -1){
      /* El segmento existe - abrir como cliente */
      if((shmid = shmget(key,sizeof(ZonaMensajes)*TAM_MEMORIA,0)) == -1){
	       perror("shmget");
	       exit(1);
      }
      else{
        printf("Yo soy el consumidor y me ligue a la memoria\n");
      }
  }
  else{
    printf("Yo soy el consumidor y cree la memoria\n");
  }

  //se ata a la zona de memoria creada
  if((Memoria = (ZonaMensajes*)shmat(shmid,(ZonaMensajes*) 0,0)) == (void *)-1) { 
      perror("shmat");
      exit(1);
  }
  //printf("%s\n",Memoria); //lee en memoria compartida

  return shmid; //regresa el identificador de la memoria
}


void DestruyeMemoriaCompartida( int id_Mem,ZonaMensajes*buffer)
{
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

//Semaforos

void waitS(int posicion){
	//printf("wait: %d \n", posicion);
	operacion.sem_num = posicion;
	operacion.sem_op = -1;
	operacion.sem_flg = SEM_UNDO;
	if(semop(semid,&operacion,1) == -1){
		printf("error wait\n");
	}
}

void signalS(int posicion){
	//printf("signal: %d \n", posicion);
	operacion.sem_num = posicion;
	operacion.sem_op = 1;
	operacion.sem_flg = SEM_UNDO;
	if(semop(semid,&operacion,1)==-1){
		printf("error signal\n");
	}
}
void *consumidor(void *arg){
	int bandera;
	char compania;
	int usuario;
	char mensaje[20];
	int filaActual=0;
	int numConsumidos=1;
	while(1){
		//Vemos que usuarios es
		if(Memoria[filaActual].usuario== 0){
			usuario1++;
		}else if(Memoria[filaActual].usuario== 1){
			usuario2++;
		}else{
			usuario3++;
		}
		//Cuando se pueda escribir cerramos el semaforo
		waitS(filaActual+TAM_MEMORIA);
		if(Memoria[filaActual].bandera==0){		
			printf(" Valor Semaforo Consumidor: %d\n",semctl(semid,filaActual+TAM_MEMORIA,GETVAL,NULL));
			//Llenamos la zona de memoria
				usuario=Memoria[filaActual].usuario;
				compania=Memoria[filaActual].compania;
				strcpy(mensaje,Memoria[filaActual].mensajes);
				printf("\nUsuario:%d Compania: %c Mensaje: %s\n",usuario,compania,mensaje);
				if(compania=='I'){
					numConsumidos++;
				}				
				Memoria[filaActual].bandera=1;
			signalS(filaActual);
		}else{
			signalS(filaActual+TAM_MEMORIA);
		}
		printf(" Valor Semaforo Productor: %d\n",semctl(semid,filaActual,GETVAL,NULL));
		//Salimos del bucle de espera activa, para pasar a la sig. iteración del ciclo mayor
		filaActual++;
		if(filaActual==(TAM_MEMORIA)){
			filaActual = 0;
		}
	printf("Consumidos: %d Usuario: %d",numConsumidos,usuario);
	}//bucle true
	pthread_exit(NULL);
}

//-------- MAIN ----------

int main(int argc, char const *argv[]){
	key_t key;
	pthread_t id_usuarios[NUM_CONS];
	ZonaMensajes mensaje[NUM_CONS];
	char sms[20];
	usuario1=0;
	usuario2=0;
	usuario3=0;


	/* create a unique key */
	if((key = ftok("/bin/ls",'K')) == -1){
		perror("ftok");
		exit(1);
	}

	//----------Crear e inicializar semaforos----------------------

	/* create a semaphore set with 8 semaphores */
	if((semid = semget(key,NUM_SEM,0666 | IPC_CREAT | IPC_EXCL)) == -1){
		if((semid = semget(key,2,0666))==-1){
			perror("semget");	
			exit(1);	
		}
		else{
			printf("Yo soy el consumidor y me ligue al semaforo\n");

		}
	}
	else{
		//Inicializacion de los semaforos
		for(int i=0;i<8;i++){
			if(i<4){
				semctl(semid,i,SETVAL,1);
			}
			else{
				semctl(semid,i,SETVAL,0);
				}	
		}
		printf("\nInicialize los semaforos, Consumidor");
	}
	//---------Crear e inicializar la memoria------------------
		shmid = CreaLigaMemoria(key);
  //Creacion de hilos 
    for(int j=0;j<NUM_CONS;j++){  //Creacion de los hilos productores
    	pthread_create(&id_usuarios[j],NULL,consumidor,NULL);
    }


    for(int j=0;j<NUM_CONS;j++){
      pthread_join(id_usuarios[j],NULL);
    }

    printf("\nusuario1: %d usuario2: %d usuario3: %d \n",usuario1,usuario2,usuario3);

    DestruyeMemoriaCompartida(shmid,Memoria);
	return 0;
}