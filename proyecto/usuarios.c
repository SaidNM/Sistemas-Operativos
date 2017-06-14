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
#include <time.h>
#include <string.h>
#define NUM_SEM 8
#define NUM_USUARIOS 3
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
        printf("Yo soy el productor y me ligue a la memoria\n");
      }
  }
  else{
    printf("Yo soy el productor y cree la memoria\n");
  }

  //se ata a la zona de memoria creada
  if((Memoria = (ZonaMensajes*)shmat(shmid,(ZonaMensajes*) 0,0)) == (void *)-1) { 
      perror("shmat");
      exit(1);
  }
  //printf("%s\n",Memoria); //lee en memoria compartida

  return shmid; //regresa el identificador de la memoria
}

void DesligoMemoriaCompartida(int id_Mem,ZonaMensajes *buffer){
    shmdt (buffer);//des asocia la memoria compartida de la zona de datos de nuestro programa
    printf("\nSe Desligo de la memoria\n");
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
void *productor(void *arg){
	ZonaMensajes *mensajeRe=(ZonaMensajes*)arg;
	char companiaR=mensajeRe->compania;
	int usuarioR=mensajeRe->usuario;
	char mensajeR[20];
	strcpy(mensajeR,mensajeRe->mensajes);
	int filaActual=0;
	int numProducidos=0;

	while(numProducidos<vueltas){
		//Cuando se pueda escribir cerramos el semaforo
		waitS(filaActual);
		//Llenamos la zona de memoria
			printf("\nUsuario: %d Fila: %d Valor bandera: %d",usuarioR,filaActual,Memoria[filaActual].bandera);
			printf(" Valor Semaforo Productor: %d\n",semctl(semid,filaActual,GETVAL,NULL));
			if(Memoria[filaActual].bandera==1){
				if(usuarioR== 0){
					usuario1++;
				}else if(usuarioR == 1){
					usuario2++;
				}else{
					usuario3++;
				}
				Memoria[filaActual].usuario=usuarioR;
				Memoria[filaActual].compania=companiaR;
				strcpy(Memoria[filaActual].mensajes,mensajeR);
				if(Memoria[filaActual].compania=='I'){
					numProducidos++;
				}
				Memoria[filaActual].bandera=0;
				if(companiaR=='T'){
					companiaR='M';
				} 
				else if(companiaR=='M'){
					companiaR='I';
				}
				else{
					companiaR='T';
				}
				signalS((filaActual+TAM_MEMORIA));
			}
			else{
				signalS(filaActual);
			}
			printf(" Valor Semaforo Consumidor: %d\n",semctl(semid,filaActual+TAM_MEMORIA,GETVAL,NULL));
			printf("Producidos: %d Usuario: %d\n",numProducidos,usuarioR);
			//Salimos del bucle de espera activa, para pasar a la sig. iteración del ciclo mayor
			filaActual++;
			if(filaActual==(TAM_MEMORIA)){
				filaActual = 0;
			}
		}//bucle true
		pthread_exit(NULL);
}

//-------- MAIN ----------

int main(int argc, char const *argv[]){
	key_t key;
	pthread_t id_usuarios[NUM_USUARIOS];
	ZonaMensajes mensaje[NUM_USUARIOS];
	char sms[20];
	usuario1=0;
	usuario2=0;
	usuario3=0;

	printf("Numero de Vueltas: ");
	scanf("%d",&vueltas);

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
			printf("Yo soy el productor y me ligue al semaforo\n");

		}
	}
	else{
		//Inicializacion de los semaforos;
		for(int i=0;i<8;i++){
			if(i<4){
				semctl(semid,i,SETVAL,1);
			}
			else{
				semctl(semid,i,SETVAL,0);
				}	
		}
		printf("\nInicialize los semaforos, Productor\n");
	}
	//---------Crear e inicializar la memoria------------------
		shmid = CreaLigaMemoria(key);

	for(int z=0;z<TAM_MEMORIA;z++){
		Memoria[z].bandera=1;
	}

	strcpy(sms,"Mensaje Registrado");
  //Creacion de hilos 
    for(int j=0;j<NUM_USUARIOS;j++){  //Creacion de los hilos productores
    	mensaje[j].compania='T';
    	mensaje[j].usuario=j;
    	strcpy(mensaje[j].mensajes,sms);
        pthread_create(&id_usuarios[j],NULL,productor,(void *)&mensaje[j]);
    }


    for(int j=0;j<NUM_USUARIOS;j++){
      pthread_join(id_usuarios[j],NULL);
    }

    printf("\nusuario1: %d usuario2: %d usuario3: %d\n",usuario1,usuario2,usuario3);
    DesligoMemoriaCompartida(shmid,Memoria);
	return 0;
}