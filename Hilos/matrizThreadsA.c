#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>

typedef struct{
	int **mA;
	int **mB;
	int **mC;
	int fila2;
	int col2;
	int lim_inferior;
	int lim_superior;
	int sup_aux;
}Matrices;

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
void *multiplicacion_matrices(void* arg){
	Matrices *parametros = (Matrices *)arg;
	int limite_inferior=parametros->lim_inferior;
	int limite_superior=parametros->lim_superior;
	int colm2=parametros->col2;
	int filam2=parametros->fila2;
	int **matrizA=parametros->mA;
	int **matrizB=parametros->mB;
	int auxfilaR=parametros->sup_aux;
	int x;
	//printf("\nlimi: %d",parametros->sup_aux);

	for(int j=0,x=limite_inferior;j<auxfilaR;j++,x++){
		for(int k=0;k<colm2;k++){
			parametros->mC[j][k]=0;
			for(int z=0;z<filam2;z++){
				parametros->mC[j][k]=parametros->mC[j][k]+(matrizA[x][z]*matrizB[z][k]);
			}
		}
	}
	//imprimir(parametros->mC,auxfilaR,colm2);
	//printf("\nlimi: %d",parametros->sup_aux);
	pthread_exit((void *)arg);
}

int main(int argc, char const *argv[]){
	//--------------CREACION DE LAS MATRICES---------------
		//variables matrices
	int **matrizA;
	int **matrizB;
	int **matrizR;
	int filam2,colm2;
	int filam1,colm1;
	int limite_inferior;
	int limite_superior;
		//variables hilos
	int numhilos;
	int tarea;
	//int tarea=filam1/numhilos; 
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

	//----------Threads--------------------
	pthread_t idphilo[numhilos];
	Matrices param[numhilos];
	void *parametrosR;
	
	for(int j=0;j<numhilos;j++){
		param[j].mA=matrizA;
		param[j].mB=matrizB;

		limite_inferior=((j+1)*tarea)-tarea;
		limite_superior=(tarea*(j+1))-1;
		if(filam1%numhilos>0){
			if(j==(numhilos-1)){
				limite_superior=filam1-1;
			}
		}
		param[j].lim_inferior=limite_inferior;
		param[j].lim_superior=limite_superior;
		param[j].fila2=filam2;
		param[j].col2=colm2;

		int filaR=(limite_superior+1)-limite_inferior;
		param[j].sup_aux=filaR;
		int **auxMatrizR=crear(auxMatrizR,filaR,colm2);
		param[j].mC=auxMatrizR;

		pthread_create(&idphilo[j],NULL,multiplicacion_matrices,(void *)&param[j]);
	}

	//Esperamos a lo hilitos...
	for(int j=0;j<numhilos;j++){
		pthread_join(idphilo[j],&parametrosR);
		Matrices *matrizResultante=(Matrices*)parametrosR;

		for(int x=(matrizResultante->lim_inferior),i=0;x<(matrizResultante->lim_superior),i<(matrizResultante->sup_aux);x++,i++){
					matrizR[x]=(matrizResultante->mC[i]);
		}

	}
	imprimir(matrizR,filam1,colm2);
	//tiempo de ejecucion
	end=clock();
	double cpu=((double)(end-start))/CLOCKS_PER_SEC;
	printf("Tiempo: %.3f\n",cpu);

	return 0;
}