#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <linux/stat.h>

//================== FUNCIONES ==========================
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
	for(int x=0;x<filas;x++){
		for(int y=0;y<columnas;y++){
			printf("%d",matriz[x][y]);
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
//============================== MAIN ========================
int main(int argc, char const *argv[]){
	srand(time(NULL));
	//--------------CREACION DE LAS MATRICES---------------
		//variables matrices
	int **matrizA;
	int **matrizB;
	int **matrizR;
	int filam2,colm2;
	int filam1,colm1;
		//variables procesos
	int numprocesos;
	pid_t proceso;
	int status=0;
	int tarea; 
		//pipes
	int fd[2];
	int terminado=0;
	//-----------Matriz A------------------
	printf("Matriz A\nfilas: ");
	scanf("%d",&filam1);
	printf("\nColumnas: ");
	scanf("%d",&colm1);

	matrizA=crear(matrizA,filam1,colm1);
	printf("\n");
	imprimir(matrizA,filam1,colm1);
	printf("\n");

	//------------Matriz B -----------------
	printf("Matriz B\nfilas: ");
	scanf("%d",&filam2);
	printf("\nColumnas: ");
	scanf("%d",&colm2);

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

	//-------------------------PROCESOS --------------------
	printf("\nNumero de procesos: ");
	scanf("%d",&numprocesos);

	tarea=filam1/numprocesos;

	for(int i=1;i<=numprocesos;i++){
		pipe(fd);
		proceso=fork();
		if(proceso==(pid_t)0){    // Soy el hijo 
		//Realizando la multiplicacion de las matrices	
			int lim_inferior=(i*tarea)-tarea;
			int lim_superior=(tarea*i)-1;

			if(filam1%numprocesos>0){
				if(i==numprocesos){
					lim_superior=filam1-1;
				}

			}		
			for(int j=lim_inferior;j<=lim_superior;j++){
				for(int k=0;k<colm2;k++){
					matrizR[j][k]=0;
					for(int z=0;z<filam2;z++){
						matrizR[j][k]=matrizR[j][k]+(matrizA[j][z]*matrizB[z][k]);
						//printf("\nA-posicion(%d,%d)=%d B-posicion(%d,%d)= %d === %d -----Soy parte del hijo %d",j,z,matrizA[j][z],z,j,matrizB[z][j],matrizR[j][k],i);
					}
					printf("\n%d %d %d --soy parte del hijo %d \n",j,k,matrizR[j][k],i);
				}
			}

			//Enviando informacion por el pipe
			/*
			close(fd[0]);
			for(int j=(i*tarea)-tarea;j<(tarea*i)-1;j++){
				for(int k=0;k<colm2;k++){
					int resultado[3];
					resultado[0]=j;
					resultado[1]=k;
					resultado[2]=matrizR[j][k];
					write(fd[1],resultado,sizeof(resultado));
				}
			}
				close(fd[1]);*/
				break;
		}
		else if(proceso>(pid_t)0){//soy el padre
			//close(fd[1]);
			if(i==numprocesos-1){
				while(wait(&status)>0);
			}
			/*int resultado[3];
			while(read(fd[0],resultado,sizeof(resultado)>0)){
				matrizR[resultado[0]][resultado[1]]=resultado[2];
			}
			terminado++;*/
			//if(terminado==numprocesos){
			//imprimir(matrizR,filam1,colm2);
			//}


		}
		else{
			printf("Error");
			exit(1);
		}
	}
	return 0;
}