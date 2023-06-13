/*                                                                                  
 * Author: Hector Robles Martinez (hector290601 on github)
 * Elaboration date: 2023.06.07
 * Goal: Write a program that implement a client-server architecture to allow a client "find" a random number from the server.
 * Specific goal: Write the server-side.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>

#define SERVER_QUEUE_NAME   "/final_project_robles_hector"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main (int argc, char **argv){
    srand(time(NULL));  // Semilla para generar el n\'{u}mero aleatorio.
    mqd_t qd_server, qd_client;   // Descriptores de la cola.
    long token_number = (int)(rand()%10000); // Numero buscado
    printf ("The number is: %ld\n", token_number);  // Informacion para el usuario.
    struct mq_attr attr;  // Creacion de la interfaz de servidor.
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    char in_buffer [MSG_BUFFER_SIZE];
    char number_buffer [MSG_BUFFER_SIZE];
    char out_buffer [MSG_BUFFER_SIZE];
    int number_test = 0;
    sprintf (in_buffer, "%s", "/final-robles-client");
    while (1) {
        if (mq_receive (qd_server, number_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Server: mq_receive");
            exit (1);
        }
	sscanf(number_buffer, "%d", &number_test);
        printf ("Server: message received:%s\n", number_buffer);
        if ((qd_client = mq_open (in_buffer, O_WRONLY)) == 1) {
            perror ("Server: Not able to open client queue");
            continue;
        }
	if(number_test < token_number){
	        sprintf (out_buffer, "%s", ">");
	}else if(number_test > token_number){
	        sprintf (out_buffer, "%s", "<");
	}else{
	        sprintf (out_buffer, "%s", "=");
	}
        if (mq_send (qd_client, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
            perror ("Server: Not able to send message to client");
            continue;
        }
        printf ("Server: response sent to client.\n");
    }
}
