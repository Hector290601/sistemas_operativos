/*
* Author: Hector Robles (hector290601 on github)
* Fecha de elaboracion: 2023.03.04
* Proposito: Escribir 'Hello world' en la saldia std.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int arbitro = 0;
int tiradas[2] = {0, 0};

void *tiradaA(void *vargp){
	int *myid = (int *)vargp;
	while(arbitro == 0){
		printf("waiting@A\n");
	}
	arbitro = 0;
	srand(time(NULL));
	tiradas[0] = *myid%3;
	arbitro = 1;
	return NULL;
}

void *tiradaB(void *vargp){
	int *myid = (int *)vargp;
	while(arbitro == 0){
		printf("waiting@B\n");
	}
	arbitro = 0;
	srand(time(NULL));
	tiradas[1] = *myid%3;
	arbitro = 1;
	return NULL;
}

int main(){
	arbitro = 1;
	for(int j = 0; j < 90; j++){
		pthread_t ida, idb;
		pthread_create(&ida, NULL, tiradaA, (void *)&ida);
		pthread_create(&idb, NULL, tiradaB, (void *)&idb);
		pthread_join(ida, NULL);
		pthread_join(idb, NULL);
		for(int i = 0; i < 2; i++){
			printf("%d\t",tiradas[i]);
		}
		printf("\n");
	}
	exit(0);
}
