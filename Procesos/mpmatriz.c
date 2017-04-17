#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

int main(int argc, char const *argv[])
{
	//matrices
	int a={{1,2,3,1},{1,2,3,1},{1,2,3,1},{1,2,3,1}};
	int b={{1,2,3,1},{1,2,3,1},{1,2,3,1},{1,2,3,1}};
	int filas=4;
	int columnas=4;
	//procesos
	int pn1=2;
	pid_t proceso;
	int status=0;
	int tarea=filas/pn1;
	
	for(int i=1;i<=pn1;i++){
		proceso=fork();
		if(proceso==0){ //soy el hijo
			int mresul=[tarea][columnas];
			for(int j=(i*tarea)-tarea;j<(tarea*i)-1;j++){
				for(int k=0;k<columnas;k++){
					//Pipe pero utilizamos una matriz
					for(int l=0;l<filas;l++){
						mresul[j%tarea][k]+=a[j][k]*matriz[l][k];
					}
				}
			}
		}
		else if(proceso>0){//soy el padre
			if(i==pn1-1){
				while(wait(&status)>0);
			}

		}
		else{
			printf("Error");
			exit(1);
		}

	}



	return 0;
}