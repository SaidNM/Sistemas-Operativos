#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <linux/stat.h>
#define SIZE 512

int main(int argc, char const *argv[])
{
	pid_t pid;
	int a[2],b[2],readbytes;
	char buffer[SIZE];

	//creacion de la tuberia
	pipe(a);
	pipe(b);

	//creacion del proceso hijo con el que habra comunicacion
	if((pid=fork())==0){
		close(a[1]);	//escritura se cierra				-1 escritura   -0 lectura
		close(b[0]);	//lectura se cierra     

		//el hijo hace una operacion de lectura
		while((readbytes=read(a[0],buffer,SIZE))>0)
			write(1,buffer,readbytes);		//esta escribiendo sobre terminal, salida estardar es 1
		close(a[0]);	//lectura se cierra
		
		//ahora hacemos una operacion de escritura

		strcpy(buffer,"Soy tu hijo hablando por la otra tuberia");
		write(b[1],buffer,strlen(buffer));		//esta abierta la escritura, por ahi se manda lo que tiene el pipe
		close(b[1]);

		exit(0);
	}
	else{
		close(a[0]);	//se cierra lectura
		close(b[1]);	//se cierra escritura

		//hacemos la operacion de escritura
		strcpy(buffer,"Soy el padre hablando por una tuberia");
		write(a[1],buffer,strlen(buffer));
		close(a[1]);

		//el padre hace la operacion de lectura
		while((readbytes=read(b[0],buffer,SIZE))>0)
			write(1,buffer,readbytes);
		close(b[0]);

		//el padre espera al hijo

		waitpid(pid);
	}

	return 0;
}