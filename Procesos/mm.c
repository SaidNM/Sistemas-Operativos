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

	int st;
	int terminado=0;
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

	//-------------------------PROCESOS --------------------
	printf("\nNumero de procesos: ");
	scanf("%d",&numprocesos);

	int fd[numprocesos][2];

	if(numprocesos>filam1){
		printf("\nError");
		exit(1);
	}
	tarea=filam1/numprocesos;

	for(int i=1;i<=numprocesos;i++){
		pipe(fd[i-1]);
		proceso=fork();
		if(proceso==(pid_t)0){    // Soy el hijo 
		//Realizando la multiplicacion de las matrices	
			close(fd[i-1][0]);
			int lim_inferior=(i*tarea)-tarea;
			int lim_superior=(tarea*i)-1;

			if(filam1%numprocesos>0){
				if(i==numprocesos){
					lim_superior=filam1-1;
				}

			}		
			for(int j=lim_inferior;j<=lim_superior;j++){
				for(int k=0;k<colm2;k++){
					int aux=0;
					for(int z=0;z<filam2;z++){
						aux=aux+(matrizA[j][z]*matrizB[z][k]);
					}
					matrizR[j][k]=aux;
				}
			}	

				//Enviando informacion del pipe------------------------------------------------
			
			int resultado[3];
			for(int j=lim_inferior;j<=lim_superior;j++){
				for(int k=0;k<colm2;k++){
					resultado[0]=j;
					resultado[1]=k;
					resultado[2]=matrizR[j][k];
					write(fd[i-1][1],&resultado,sizeof(resultado));
					//printf("\n%d\n",i-1);
				}
			}
					close(fd[i-1][1]);
				//-------------------------------------------------------------------------------

				//printf("\nSoy el hijo %d",i);
				break;

		}
		else if(proceso>(pid_t)0){//soy el padre
			close(fd[i-1][1]);
			//while(wait(&status)>0);

			if(i==(numprocesos)){
			int resultado_recibido[3];
			for(int k=0;k<numprocesos;k++){
				if(k>0){
					close(fd[k-1][0]);
				}
				while((st=read(fd[k][0],&resultado_recibido,sizeof(resultado_recibido)))>0){
				//st=read(fd[0],&resultado_recibido,sizeof(resultado_recibido));
				matrizR[resultado_recibido[0]][resultado_recibido[1]]=resultado_recibido[2];
				terminado++;			
				}
			}
				imprimir(matrizR,filam1,colm2);	
				printf("\n \t %d\n ",terminado);
				printf("\nBye\n")	;
			}
			
		}
		else{
			printf("Error");
			exit(1);
		}
	}
	return 0;
}