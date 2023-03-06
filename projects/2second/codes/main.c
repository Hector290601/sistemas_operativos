/*
* Author: Hector Robles (hector290601 on github)
* Fecha de elaboracion: 2023.03.04
* Proposito: Generar un hilo principal (Main) que genere dos hilos secundarias
* (tiradaA y tiradaB) que hagan un juego de 'piedra, papel o tijeras' para que
* el hilo maestro los ejecute N veces.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int arbitro = 0;
double tirada_a = 0;
double tirada_b = 0;

void *tiradaA(void *vargp){
	int *myid = (int *)vargp;
	while(arbitro == 0);
	arbitro = 0;
	srand(time(NULL));
	srand(*myid);
	tirada_a = rand();
	arbitro = 1;
	return NULL;
}

void *tiradaB(void *vargp){
	int *myid = (int *)vargp;
	while(arbitro == 0);
	arbitro = 0;
	srand(time(NULL));
	srand(*myid);
	tirada_b = rand();
	arbitro = 1;
	return NULL;
}

int main(){
	arbitro = 1;
	int ganadas_a = 0, ganadas_b = 0, empates = 0;
	for(int i = 0; i < 100; i++){
		pthread_t idb;
		pthread_t ida;
		pthread_create(&ida, NULL, tiradaA, (void *)&ida);
		pthread_create(&idb, NULL, tiradaB, (void *)&idb);
		pthread_join(ida, NULL);
		pthread_join(idb, NULL);
		arbitro = 0;
		if(tirada_a > tirada_b){
			ganadas_a ++;
		}else if(tirada_a < tirada_b){
			ganadas_b ++;
		}else{
			empates++;
		}
		arbitro = 1;
	}
	printf("Ganadas de A:%i\tGanadas de B:%i\tEmpates:%i\n", ganadas_a, ganadas_b, empates);
	exit(0);
}
