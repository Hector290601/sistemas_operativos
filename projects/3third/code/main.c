#include <stdio.h>

#define BOARD_H 8
#define BOARD_W 8

char board[BOARD_H][BOARD_W];

int main(){
	for(int i = 0; i < BOARD_H; i++){
		for(int j = 0; j < BOARD_W; j++){
			board[i][j] = ' ';
		}
	}
	printf("\033[40;33m +");
	for(int j = 0; j < BOARD_W; j++){
		printf("-");
	}
	printf("+\033[0m\n");
	for(int i = 0; i < BOARD_H; i++){
		printf("\033[40;33m%i|\033[0m", 8-i);
		for(int j = 0; j < BOARD_W; j++){
			if((i+1)%2 != 0){
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
	printf("\033[0m\n  ");
	return 0;
}
