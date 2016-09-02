#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#define DELAY 600000

int main() {	
	char cBlock = (char)0x2588;
	int rows, cols, sum = 0;
	
	srand (time(NULL)); 
	
	initscr();									
	curs_set(FALSE); 							
	
	getmaxyx(stdscr, rows, cols); 				
	rows = rows - 1;
	
	int matA[rows][cols];
	int matB[rows][cols];
	
	for (int i = 0; i < rows; i++) {			
		for (int j = 0; j < cols; j++ ) {
			matA[i][j] = rand() % 2;
		}
	}
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++ ) {
			if (matA[i][j] == 1) { 
				mvaddch(i, j, cBlock);			
			}
		}
	}
		
	refresh();									
	getch();
	int g = 1;
	while (1) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j ++) {
				sum = 0;
				for (int r = (i - 1); r <= i + 1; r++) { 
					for (int c = j - 1; c <= j + 1; c++) {
						if (!((r == i) && (c == j))) {
							if ((r >= 0) && (r < rows)) {
								if ((c >= 0) && (c < cols)) {
									sum = sum + matA[r][c];
								}
							}
						}
					}
				}
				if (matA[i][j] == 0) {
					if (sum == 3) { 				//Nacimiento
						matB[i][j] = 1;
					}
				} else {
					matB[i][j] = 1;
					if (sum < 2) {
						matB[i][j] = 0; 			//muerte por soledad
					}
					if (sum > 3) {
						matB[i][j] = 0;				//muerte por superpoblacion
					}
				}
				
			}
		}
	
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				matA[i][j] = matB[i][j];
			}
		}
		
		clear();
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++ ) {
				if (matA[i][j] == 1) { 
					mvaddch(i, j, cBlock);			
				}
			}
		}
		mvprintw (rows, 1, "Generacion %d", g++);
		refresh();									
		usleep(DELAY);
		
	}		
	endwin();
	return 0;
}
