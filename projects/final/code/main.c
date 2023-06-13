/*
* Author: Hector Robles Martinez (hector290601 on github)
* Elaboration date: 2023.03.04
* Goal: Simulate a cheese game abor the queen following the king using threads, standard cheese rules and global variables.
*/
#include <stdio.h>	// Library to manage the stdout
#include <stdlib.h>	// Library to use the 'system' function
#include <unistd.h>	// Library to use the 'pause' function
#include <pthread.h>	// Library to manage the threads

#define BOARD_H 8	// Define board height
#define BOARD_W 8	// Define board width
#define TIME_SLEEP 1	// Define pause time

char board[BOARD_H][BOARD_W];	// Create board array
int king_current_w = 4;		// Variable to store the current king position on widht
int king_current_h = 0;		// Variable to store the current king position on height
int queen_current_w = 3;	// Variable to store the current queen position on width
int queen_current_h = 7;	// Variable to store the current queen position on height
int delta_w = 0;		// Variable to store the diference between the king's width and queen's width
int delta_h = 0;		// Variable to store the diference between the king's height and queen's height
int movements_king = 0;		// Variable to store the king's movements count
int movements_queen = 0;	// Variable to store the queen's movements count
int available = 1;

/*
* Resume {{{
* Functions named get_delta_*:
*	Calculates the delta between tehe king's and queen's positions on the specified position.
* Functions named *_movement:
*	Control the movements on the specified piece, the algoright is the netx:
*		Reset the board position to the blank position.
*		Then, looks for the lower delta to decide which position is more convenient to change.
*		Once decided wich position is more convenient to change, it moves following the
*		cheese rules defined to the piece.
*		Then increment by one the piece movenet's counter
* Function named initialize_board:
*	Initialice the board to blank values, ' ', to clean the reserver memory.
* Function named print_board:
*	Print the board variable, this function even make the color changes using the ANSI scape codes,
*	this initialice with the '\033' scape code, then use the '[', then use a two digit color code,
*	used to set the backgroun color, then a semicolon ';' following by another two digit color code,
*	used to set the font color, then a 'm' letter, this is defined as special characters on the ANSI
*	standard, once the background and font colors are setted, print the stored value on the board
*	array.
* Function named main:
*	Run the initialize board function, then print the board after the first movement, calculates the
*	deltas on the two dimentions, then wait a TIME_SLEEP period, then enter to a while loop, to run
*	the followuing steps to run the program:
*		Verify the deltas are different to 0, if this is true, it means the king and queen are
*		on different positions, this means the game can continue, then create the variables needed
*		to create the thread, named id_*, then recalculate the deltas, then creates the threads
*		using the pthread_create function provided by the pthread.h library, then run the threads
*		and wait them to end using the pthread_join function, then print the board again to wait
*		a 'TIME_SLEEP' period, then clear the screen to repeat the steps.
*	Once the deltas are 0, print again the board and print the information about where and how many
*	times the program can run before the queen kill the king and the program end.
* }}}
*/

// get delta x{{{
void get_delta_w(){
	delta_w = queen_current_w - king_current_w;
	if(delta_w < 0){
		delta_w = -1 * delta_w;
	}
}
//}}}

// get delta y{{{
void get_delta_h(){
	delta_h = queen_current_h - king_current_h;
	if(delta_h < 0){
		delta_h = -1 * delta_h;
	}
}
//}}}

// king movement [*king_movement]{{{
void* king_movement(void* vargp){
	while(delta_w != 0 && delta_h != 0 && available){
		available = 0;
		board[king_current_h][king_current_w] = ' ';
		if(delta_w < delta_h){
			//mover a w
			if(delta_w == 0){
				king_current_h++;
			}else{
				king_current_w++;
			}
		}else if(delta_h < delta_w){
			//mover a h
			if(delta_h == 0){
				king_current_w++;
			}else{	
				king_current_h++;
			}
		}
		movements_king++;
		get_delta_w();
		get_delta_h();
		available = 1;
	}
	return NULL;
}
//}}}

// queen movement [*void queen_movement]{{{
void* queen_movement(void* vargp){
	while(delta_w != 0 && delta_h != 0 && available){
		available = 0;
		board[queen_current_h][queen_current_w] = ' ';
		if(delta_w < delta_h){
			//mover a w
			if(delta_w == 0){
				queen_current_h = king_current_h;
			}else{
				queen_current_w = king_current_w;
			}
		}else if(delta_h < delta_w){
			//mover a h
			if(delta_h == 0){
				queen_current_w = king_current_w;
			}else{
				queen_current_h = king_current_h;
			}
		}
		movements_queen++;
		get_delta_w();
		get_delta_h();
		available = 1;
	}
	return NULL;
}
//}}}

// initialize board {{{
void initialize_board(){
	for(int i = 0; i < BOARD_H; i++){
		for(int j = 0; j < BOARD_W; j++){
			board[i][j] = ' ';
		}
	}
}
//}}}

// print board {{{
void print_board(){
	board[king_current_h][king_current_w] = 'K';
	board[queen_current_h][queen_current_w] = 'Q';
	printf("\033[40;33m +");
	for(int j = 0; j < BOARD_W; j++){
		printf("-");
	}
	printf("+\033[0m\n");
	for(int i = 0; i < BOARD_H; i++){
		printf("\033[40;33m%i|\033[0m", i);
		for(int j = 0; j < BOARD_W; j++){
			if(i == king_current_h && j == king_current_w){
				if((i+1)%2 != 0){
					if((j+1)%2 != 0){
						printf("\033[47;35m%c\033[0m", board[i][j]);
					}else{
						printf("\033[40;35m%c\033[0m", board[i][j]);
					}
				}else{
					if((j+1)%2 == 0){
						printf("\033[47;35m%c\033[0m", board[i][j]);
					}else{
						printf("\033[40;35m%c\033[0m", board[i][j]);
					}
				}
			}else if(i == queen_current_h && j == queen_current_w){
				if((i+1)%2 != 0){
					if((j+1)%2 != 0){
						printf("\033[47;31m%c\033[0m", board[i][j]);
					}else{
						printf("\033[40;31m%c\033[0m", board[i][j]);
					}
				}else{
					if((j+1)%2 == 0){
						printf("\033[47;31m%c\033[0m", board[i][j]);
					}else{
						printf("\033[40;31m%c\033[0m", board[i][j]);
					}
				}
			}else if((i+1)%2 != 0){
				if(j%2 != 0){
					printf("%c\033[40;33m\033[0m", board[i][j]);
				}else{
						printf("\033[47;30m%c\033[40;33m\033[0m", board[i][j]);
				}
			}else{
				if(j%2 == 0){
					printf("%c\033[40;33m\033[0m", board[i][j]);
				}else{
					printf("\033[47;30m%c\033[40;33m\033[0m", board[i][j]);
				}
			}
		}
		printf("\033[40;33m|\n");
	}
	printf("\033[40;33m +");
	for(int j = 0; j < BOARD_W; j++){
		printf("-");
	}
	printf("+\n  ");
	for(int j = 0; j < BOARD_W; j++){
		printf("%c", 65 + j);
	}
	printf("\033[0m\n");
}
// }}}

// main {{{
int main(){
	initialize_board();
	print_board();
	get_delta_w();
	get_delta_h();
	sleep(TIME_SLEEP);
	system("clear");
	pthread_t id_king;
	pthread_t id_queen;
	pthread_create(&id_king, NULL, *king_movement, NULL);
	pthread_create(&id_queen, NULL, *queen_movement, NULL);
	while(delta_w != 0 && delta_h != 0){
		if(delta_w != 0 && delta_h != 0){
			// pthread_join(id_king, NULL);
			// pthread_join(id_queen, NULL);
		}
		print_board();
		sleep(TIME_SLEEP);
		system("clear");
	}
	print_board();
	printf("King has dead!\n");
	printf("K@(%i, %i); ", king_current_w, king_current_h);
	printf("Q@(%i, %i)\n", queen_current_w, queen_current_h);
	printf("King achieve %i movements\n", movements_king);
	printf("Queen achieve %i movements\n", movements_queen);
	return 0;
}
// }}}
