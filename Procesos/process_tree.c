#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void crear_hijo(int nivel,int num_hijos, int pos, int status, int paro, int hijos){
	if(paro==nivel){
		exit(0);
	}
	for(int i=0;i<num_hijos;i++){
	int pid1=fork();
	int status=0;
	if(pid1>0){
		hijos++;
		while(wait(&status)>0);
	}
	else if(pid1==0){
		nivel++;
		if(pos==0){
			printf("\t Soy hijo -mi pid es %d - mi padre es %d - mi nivel es %d ---Izquierdo \n",getpid(),getppid(),nivel);	
			}
		else{
		printf("\t \t Soy hijo -mi pid es %d - mi padre es %d - mi nivel es %d  ---Derecho \n",getpid(),getppid(),nivel);
			}
		crear_hijo(nivel,num_hijos,pos,status,paro,hijos);
		break;
		}
	else{
		printf("\nError");
		exit(1);
	}
	}
} 
//---------------------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[])
 {
 	int pid=0;
 	int nivel=0;
 	int status=0;
 	int paro=0;
 	int hijosc=0;
 	int num_hijos_izq=0;
 	int num_hijos_der=0;

 	printf("\nNum hijos izquierdo: ");
 	scanf("%d",&num_hijos_izq);
 	if(num_hijos_izq<0){
 		printf("\nError");
 		exit(1);
 	}
 	printf("\nNum hijos derecho: ");
 	scanf("%d",&num_hijos_der);
 	if(num_hijos_der<0){
 		printf("\nError");
 		exit(1);
 	}
 	printf("\nNiveles: ");
 	scanf("%d",&paro);
 	if(paro<0){
 		printf("\nError");
 		exit(1);
 	}

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
	 			crear_hijo(nivel,num_hijos_izq,i,status,paro,hijosc);
	 		}
	 		else{//Es el hijo derecho
				nivel++;
	 			printf("\t Soy el hijo derecho -mi pid es %d - mi padre es %d - mi nivel es %d \n",getpid(),getppid(),nivel);
	 			crear_hijo(nivel,num_hijos_der,i,status,paro,hijosc);
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
