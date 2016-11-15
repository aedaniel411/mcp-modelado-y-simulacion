/*
 * Rösler Attractor
 */
 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define T 0.01

int main () {
	int i;
	double x0, y0, z0, x1, y1, z1;
	double a, b, c;
	FILE *fp = fopen("ros.dat", "w");
	
	x0 = y0 = z0 = 1.0;
	
	a = b = 0.2;
	c = 9;
	
	for (i=0; i < 50000; i++) {
		x1 = x0 + T *((-y0) - z0);
		y1 = y0 + T * ((x0 + a * y0));
		z1 = z0 + T *(b + z0 * (x0 - c));
		
		x0 = x1; 
		y0 = y1; 
		z0 = z1;
		
		fprintf (fp, "%lf %lf %lf\n", x1, y1, z1);
	}
	
	fclose(fp);
	
	return 0;
}
