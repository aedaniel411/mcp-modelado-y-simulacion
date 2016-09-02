#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

//#define DELAY 600000
#define DELAY 1

void inicializar (int *mat, int rows, int cols) {
	for (int i = 0; i < rows; i++) {			
		for (int j = 0; j < cols; j++ ) {
			mat[i * cols + j] = rand() % 2;
		}
	}
}

void mostrar (int *mat, int rows, int cols) {
	char cBlock = (char)0x2588;
	
	for (int i = 0; i < rows; i++) {			
		for (int j = 0; j < cols; j++ ) {
			if (mat[i * cols + j] == 1) { 
				mvaddch(i, j, cBlock);			
			}
		}
	}
}

int sumvivos (int *mat, int row, int rows, int col, int cols) {
	int sum = 0;
	
	for (int r = (row - 1); r <= row + 1; r++) { 
		for (int c = col - 1; c <= col + 1; c++) {
			if (!((r == row) && (c == col))) {
				if ((r >= 0) && (r < rows)) {
					if ((c >= 0) && (c < cols)) {
						sum = sum + mat[r * cols + c];
					}
				}
			}
		}
	}
	
	return sum;
} 

int viveomuere (int actual, int vivos) {
	int siguiente = 0;
	if (actual == 0) {
		if (vivos == 3) {				//Nacimiento
			siguiente = 1;
		}
	} else {
		siguiente = 1;
		if (vivos < 2) {
			siguiente = 0; 			//muerte por soledad
		}
		if (vivos > 3) {
			siguiente = 0;				//muerte por superpoblacion
		}
	}
	
	return siguiente;
}

int main() {	
	int rows, cols, sum = 0;
	
	srand (time(NULL)); 
	
	initscr();									
	curs_set(FALSE); 							
	
	getmaxyx(stdscr, rows, cols); 				
	rows = rows - 1;
			
	int *matA, *matB; 
	matA = (int *)calloc( cols * rows, sizeof(int));
	
	inicializar (matA, rows, cols);
	
	mostrar (matA, rows, cols);
		
	refresh();									
	getch();
	
	int g = 1;
	while (1) {
		
		matB = (int *)calloc( cols * rows, sizeof(int));
		
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j ++) {
				sum = sumvivos (matA, i, rows, j, cols);
				
				matB [i * cols + j] = viveomuere (matA [i * cols + j], sum);
			}
		}
		
		free(matA);
		matA = matB;

		clear();
		mostrar (matA, rows, cols);
		mvprintw (rows, 1, "Generacion %d", g++);
		refresh();			
								
//		usleep(DELAY);
		sleep(DELAY);	
		
	}
	
	free (matB);
	endwin();
	return 0;
}
