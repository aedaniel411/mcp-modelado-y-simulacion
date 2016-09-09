#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>
#include <ncurses.h>

#include <curand_kernel.h>
#include <curand.h>

//#define DELAY 600000
#define DELAY 1

__global__ void inicializar (int *mat, int rows, int cols, unsigned int seed) {
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int ren = blockIdx.y * blockDim.y + threadIdx.y;
	int index = ren * cols + col;

	curandState_t state;
	curand_init (seed, index, 0, &state);

	mat[index] = curand (&state) % 2;
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

__device__ int sumvivos (int *mat, int row, int rows, int col, int cols) {
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

__device__ int viveomuere (int actual, int vivos) {
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

__global__ void nuevaGeneracon (int *matOLD, int *matNEW, int rows, int cols) {
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int ren = blockIdx.y * blockDim.y + threadIdx.y;
	int index = ren * cols + col;
	int sum = 0;

	sum = sumvivos (matOLD, ren, rows, col, cols);
	matNEW[index] = viveomuere(matOLD[index], sum);
}

int cuantosBloques(int N, int hilos) {
	int c, m = N % hilos;
	if (m > 0) {
		c = (N + hilos) / hilos;
	} else {
		c = N / hilos;
	}
	return c;
}

int main() {
	int rows, cols; //, sum = 0;

	initscr();
	curs_set(FALSE);

	getmaxyx(stdscr, rows, cols);
	rows = rows - 1;

	int *matA;
	matA = (int *)calloc( cols * rows, sizeof(int));

	int *dmatA, *dmatB;
	cudaMalloc((void**)&dmatA, cols * rows * sizeof(int));

	dim3 dimThreadsBloque(16, 16);

	dim3 dimBloques(cuantosBloques(cols,16), cuantosBloques(rows,16));

	inicializar <<<dimBloques, dimThreadsBloque>>> (dmatA, rows, cols, time(NULL));

	cudaMemcpy(matA, dmatA, cols * rows * sizeof(int), cudaMemcpyDeviceToHost);

	mostrar (matA, rows, cols);

	refresh();
	getch();

	int g = 1;

	while (1) {
		cudaMalloc((void**)&dmatB, cols * rows * sizeof(int));

		nuevaGeneracon <<<dimBloques, dimThreadsBloque>>> (dmatA, dmatB, rows, cols);

		cudaMemcpy(matA, dmatB, cols * rows * sizeof(int), cudaMemcpyDeviceToHost);

		dmatA = dmatB;
        cudaFree (dmatB);
		clear();
		mostrar (matA, rows, cols);
		mvprintw (rows, 1, "Generacion %d", g++);
		refresh();

//		usleep(DELAY);
		sleep(DELAY);

	}

	free (matA);
	cudaFree(dmatA);

	endwin();
	return 0;
}
