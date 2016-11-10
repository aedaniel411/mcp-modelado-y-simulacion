/*
 * Computer Aided Simulations
 * Proram 2.4: Creating Data with C
 * Lorenz Attractor
 */
 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define T 0.001

int main () {
	int i;
	double x0, y0, z0, x1, y1, z1;
	FILE *fp = fopen("lor.dat", "w");
	
	x0 = y0 = z0 = 1.0;
	
	for (i=0; i < 50000; i++) {
		x1 = x0 + T * ((-10.0) * (x0 - y0));
		y1 = y0 + T * ((-x0) * z0 + 28.0 * x0 -y0);
		z1 = z0 + T * (x0 * y0 - 8.0 / 3.0 * z0);
		x0 = x1; 
		y0 = y1; 
		z0 = z1;
		fprintf (fp, "%lf %lf\n", x1, z1);
	}
	
	fclose(fp);
	
	return 0;
}
