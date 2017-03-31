#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
	int proceso;
	proceso=fork();				//fork -- int > 0 = id hijo --int - = error -- int 0 = es hijo

	if (proceso==0){
		printf("Soy el proceso hijo\n");
	}
	else if(proceso > 0){
		printf("Soy el proceso padre\n");
	}
	else{
		printf("Error\n")
	}

	return 0;
}