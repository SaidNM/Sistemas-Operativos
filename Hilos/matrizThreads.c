#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

//Variables Globales 
//--------------CREACION DE LAS MATRICES---------------
		//variables matrices
	int **matrizA;
	int **matrizB;
	int **matrizR;
	int filam2,colm2;
	int filam1,colm1;
		//variables procesos
	//int tarea=filam1/numhilos; 
	int j=0;
	int tarea;
	int numhilos;

//funciones generales 
int generar_numero(){
	int numero;
	numero=rand() % 3;
	return numero;
}
void llenar_matriz(int ** matriz, int filas, int columnas){
	for(int x=0;x<filas;x++){
		for(int y=0;y<columnas;y++){
			matriz[x][y]=generar_numero();
		}
	}
}
void imprimir(int ** matriz, int filas, int columnas){
	printf("\n");
	for(int x=0;x<filas;x++){
		for(int y=0;y<columnas;y++){
			printf(" %d ",matriz[x][y]);
		}
		printf("\n");
	}
}
void *crear(int **matriz,int fila,int columna){
	matriz=(int**)malloc(sizeof(int*)*fila);
	for(int x=0;x<fila;x++){
		matriz[x]=(int*)malloc(sizeof(int)*columna);
	}
	llenar_matriz(matriz,fila,columna);
	return matriz;
}
//funcion hilos
void multiplicacion(void* arg){
	int i=(int)arg;
	int limite_inferior=(i*tarea)-tarea;
	int limite_superior=(tarea*i)-1;
	//printf("inferior: %d superior %d i: %d",limite_inferior,limite_superior,i);


	if(tarea>0){
		if(i==numhilos){
			limite_superior=filam1-1;
		}
	}		
	for(int j=limite_inferior;j<=limite_superior;j++){
		for(int k=0;k<colm2;k++){
			matrizR[j][k]=0;
			for(int z=0;z<filam2;z++){
				matrizR[j][k]=matrizR[j][k]+(matrizA[j][z]*matrizB[z][k]);
			}
		}
	}
	pthread_exit(NULL);
}
//---------------MAIN---------------------
int main(int argc, char const *argv[]){
	int start;
	int end;
	start=clock();
	srand(time(NULL));
	//-----------Matriz A------------------
	printf("Matriz A\nfilas: ");
	scanf("%d",&filam1);
	printf("\nColumnas: ");
	scanf("%d",&colm1);
	if(filam1<=0 || colm1<=0){
		printf("Error");
		exit(1);
	}

	matrizA=crear(matrizA,filam1,colm1);
	printf("\n");
	imprimir(matrizA,filam1,colm1);
	printf("\n");

	//------------Matriz B -----------------
	printf("Matriz B\nfilas: ");
	scanf("%d",&filam2);
	printf("\nColumnas: ");
	scanf("%d",&colm2);

	if(filam2<=0 || colm2<=0){
		printf("Error");
		exit(1);
	}

	matrizB=crear(matrizB,filam2,colm2);
	printf("\n");
	imprimir(matrizB,filam2,colm2);
	printf("\n");

	if(colm1!=filam2){
		printf("No se puede realizar la operacion\n");
		free(matrizA);
		free(matrizB);
		exit(1);
	}

	//----------Matriz Resultante---------
	matrizR=crear(matrizR,filam1,colm2);
	
	printf(" \nNumero de hilos: ");
	scanf("%d",&numhilos);

	tarea=filam1/numhilos;
	pthread_t idphilo[numhilos];



	for(j=1;j<=numhilos;j++){
		int x=pthread_create(&idphilo[j-1],NULL,(void *)&multiplicacion,(void *)j);
		//printf("%d",x);
	}

	
	//creacion de id hilos

	for(j=0;j<numhilos;j++){
		pthread_join(idphilo[j],NULL);
	}
	imprimir(matrizR,filam1,colm2);
	end=clock();
	double cpu=((double)(end-start))/CLOCKS_PER_SEC;
	printf("Tiempo: %.3f",cpu);
	return 0;
}