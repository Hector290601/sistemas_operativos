/*
 * Author: Hector Robles Martinez (hector290601 on github)
 * Elaboration date: 2023.06.07
 * Goal: Write a program that implement a client-server architecture to allow a client "find" a random number from the server.
 * Specific goal: Write the client-side
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/final_project_robles_hector"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
int main (int argc, char **argv){
    char client_queue_name [64];
    mqd_t qd_server, qd_client;   // queue descriptors
    // create the client queue for receiving messages from server
    sprintf (client_queue_name, "/final-robles-client");
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Client: mq_open (client)");
        exit (1);
    }
    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        perror ("Client: mq_open (server)");
        exit (1);
    }
    char in_buffer [MSG_BUFFER_SIZE];
    char out_buffer [MSG_BUFFER_SIZE];
    //printf ("Ask for a token (Press <ENTER>): ");
    char temp_buf [10];
    int number = 0, min = 0, max = 10000;
    number = (int)((float)(max - min) / 2.0) + min;
    printf ("Looking for number on the next range:\n %d < %d < %d \r", min, number, max);
    while (1){//fgets (temp_buf, 2, stdin)) {
        // send message to server
	number = (int)((float)(max - min) / 2.0) + min;
	//printf("Min:%d\tMax:%d\tNum:%d\n", min, max, number);
	sprintf (out_buffer, "%d", number);
        if (mq_send (qd_server, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
            perror ("Client: Not able to send message to server");
            continue;
        }
        // receive response from server
        if (mq_receive (qd_client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Client: mq_receive");
            exit (1);
        }
        // display token received from server
        //printf ("Client: Token received from server: %s\n\n", in_buffer);
	if(in_buffer[0] == 62){
	    //printf("a\n");
            min = number + 1;
	}else if(in_buffer[0] == 60){
	    //printf("b\n");
            max = number - 1;
	}else if(in_buffer[0] == 61){
            //printf("Founded! \n");
	    printf("The number was: %d\n", number);
            break;
	}
        printf ("Looking for number on the next range: %d < %d < %d \r", min, number, max);
    }
    if (mq_close (qd_client) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }
    if (mq_unlink (client_queue_name) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    printf ("Client: bye\n");
    exit (0);
}
