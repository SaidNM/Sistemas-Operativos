#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void crear_hijo(int nivel){
	if(paro==3){
			exit(0);
		}
	int pid1=fork();
	int status=0;
	if(pid1>0){
		printf("Soy el padre - mi pid es %d -mi nivel es %d \n",getpid(),nivel);
		while(wait(&status)>0);
		crear_hijo(nivel,paro);
		
	}
	else{
		nivel++;
		printf("\t Soy hijo -mi pid es %d - mi padre es %d - mi nivel es %d \n",getpid(),getppid(),nivel);
		crear_hijo(nivel,paro);
		}
} 

 int main(int argc, char const *argv[])
 {
 	int pid=0;
 	int nivel=0;
 	int status=0;
 	int paro=0;
 	for(int i=0;i<2;i++){
 		pid=fork();
	 	if(pid>0){ //Aqui esta el codigo del padre
	 		if(i==1){
	 			printf("Soy el padre - mi pid es %d -mi nivel es %d \n",getpid(),nivel);
	 			while(wait(&status)>0);
	 		}

	 	}
	 	else if(pid==0){ //Aqui esta el codigo del hijo
	 		if(i==0){ //Es el hijo izquierdo
	 			nivel++;
	 			printf("\t Soy el hijo izquierdo -mi pid es %d - mi padre es %d - mi nivel es %d \n",getpid(),getppid(),nivel);
				//--------------------------------------------------------------------------------
				int pid1=fork();
				if(pid1>0){
					printf("Soy el padre - mi pid es %d -mi nivel es %d \n",getpid(),nivel);
	 				while(wait(&status)>0);
				}
				else{
					nivel++;
					printf("\t Soy hijo -mi pid es %d - mi padre es %d - mi nivel es %d \n",getpid(),getppid(),nivel);
	 				}
	 			//---------------------------------------------------------------------------------------
	 		}
	 		else{//Es el hijo derecho
				nivel++;
	 			printf("\t Soy el hijo derecho -mi pid es %d - mi padre es %d - mi nivel es %d \n",getpid(),getppid(),nivel);
	 			paro++;
	 			crear_hijo(nivel);
	 		}
	 		break;
	 	}
	 	else{
	 		printf("Error Error Error\n");
	 		exit(1);
	 	}
	}
 	return 0;
 }