#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define CON  4
#define PROC  5
#define SOC 6

typedef struct objeto{
	int bandera;
	char cadena[30];
	int usuario;
	char compania;
}ZonaMensajes;

typedef struct estructura{
	int numthread;
	ZonaMensajes *espacios;
}estructura;

int id_semaforo,usuario1,usuario2,usuario3,vueltas;

void * consume(void *arg);
void waitt(int definido);
void signall(int definido);
void DestruyeMemoriaCompartida(int id_Mem,ZonaMensajes *buffer);

int main(void){
	int i, compartida;
	pthread_t consumidor[2];
	usuario1=0;
	usuario2=0;
	usuario3=0;

	printf("Numero de Vueltas: ");
	scanf("%d",&vueltas);
	//---- Creación de la llave del semaforo ----//
	key_t llave_sem = ftok("/bin/rm", 'L');

	//---- Crear semaforo ----//
	if((id_semaforo = semget(llave_sem, 7, 0644 | IPC_CREAT | IPC_EXCL)) < 0){
		if((id_semaforo = semget(llave_sem, 7, 0)) < 0){
			printf("Error al ligar el semaforo\n");
		} else{
			printf("Se ligo al semaforo %d\n", id_semaforo);
		}
	} else{
		printf("Se creo correctamente el semaforo %d\n", id_semaforo);
		//---- Inicializar semaforo ----//
		semctl(id_semaforo, 0, SETVAL, 1);
		semctl(id_semaforo, 1, SETVAL, 1);
		semctl(id_semaforo, 2, SETVAL, 1);
		semctl(id_semaforo, 3, SETVAL, 1);
		semctl(id_semaforo, CON, SETVAL, 0);
		semctl(id_semaforo, PROC, SETVAL, 4);
		semctl(id_semaforo, SOC, SETVAL, 1);
	}

	//---- Creación de la llave de la memoria ----//
	int llave = ftok("/bin/ls",'M');

	//---- Crear memoria compartida o ligarse a esta ----//
	if((compartida = shmget(llave, sizeof(ZonaMensajes)*4, 0644|IPC_CREAT|IPC_EXCL)) < 0){
		if((compartida = shmget(llave, sizeof(ZonaMensajes)*4, 0644)) < 0){
			printf("Error al ligar la memoria compartida\n");
			exit(1);
		}
		else{
			printf("Se ha ligado la memoria compartida con id: %d.\n",compartida);
		}
	} else{
		printf("Se ha creado la memoria compartida con id: %d.\n",compartida);
	}

	//---- Declarar memoria ----//
	ZonaMensajes *Memoria;
	if((Memoria=shmat(compartida,0,0))==NULL){
		printf("Error al reservar memoria\n");
		exit(1);
	}

	for(i=0; i<4; i++){
		Memoria[i].bandera = 0;
	}

	estructura stru[2];
	//---- Crear hilos ----//
	for(i=0; i<2; i++){
		stru[i].numthread = i;
		stru[i].espacios = Memoria;
		if((pthread_create(&consumidor[i], NULL, consume, (void*) &stru[i])) < 0){
			printf("Error al crear el hilo\n");
		}
	}

	//---- Esperar hilos ----//
	for(i=0; i<2; i++){
		pthread_join(consumidor[i], NULL);
	}

    printf("\nusuario1: %d usuario2: %d usuario3: %d\n",usuario1,usuario2,usuario3);

    //Destruimos los semaforos
    for(int i=0;i<13;i++){
    	semctl(id_semaforo,i,IPC_RMID);
    }
    DestruyeMemoriaCompartida(compartida,Memoria);

	printf("Fin del programa, presione enter\n");
	getchar();
	return 0;
}

void *consume(void *arg){
	int i, seguir, k,lim0,lim1,limite;
	estructura *datos = (estructura*) arg;

	if(vueltas%2==0){
		lim0=(vueltas*3)/2;
		lim1=lim0;
	}else{
		lim0=(vueltas*3)/2;
		lim1=lim0+1;
	}
	if(datos->numthread==0){
		limite=lim0;
	}else{
		limite=lim1;
	}

	for(k=0; k<limite; k++){
		seguir=0;
		waitt(CON);
		while(seguir==0){
			for(i=0; i<4; i++){
				waitt(i);
				if(datos->espacios[i].bandera != 0){
					if(datos->espacios[i].usuario == 0){
						usuario1++;
					}else if(datos->espacios[i].usuario == 1){
						usuario2++;
					}else{
						usuario3++;
					}
					//waitt(SOC);
						//---- Leer memoria ----//
						printf("\nHilo: %d Consumiendo: %d",datos->numthread,k);
						printf(" Usuario: %d Compania: %c Cadena: %s\n",datos->espacios[i].usuario,datos->espacios[i].compania, datos->espacios[i].cadena);
						//---- Enviamos los datos//
						//send(canal, &datos->espacios[i], sizeof(datos->espacios[i]), 0);
						if(datos->espacios[i].compania=='I'){
						seguir=1;
						}
						datos->espacios[i].bandera=0;
						//strcpy(datos->espacios[i].cadena,"00000000000000000000");

					//signall(SOC);
					signall(i);
					i=4;
				}else{
					signall(i);
				}
			}
		signall(PROC);
		}
	}
	pthread_exit(0);
}

void waitt(int definido){
	struct sembuf operacion;
	operacion.sem_num = definido;
	operacion.sem_op = -1;
	operacion.sem_flg = SEM_UNDO;
	semop(id_semaforo, &operacion, 1);
}

void signall(int definido){
	struct sembuf operacion;
	operacion.sem_num = definido;
	operacion.sem_op = 1;
	operacion.sem_flg = SEM_UNDO;
	semop(id_semaforo, &operacion, 1);
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
//