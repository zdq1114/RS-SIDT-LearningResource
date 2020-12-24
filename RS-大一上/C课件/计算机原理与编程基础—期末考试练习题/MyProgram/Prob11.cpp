
#include <stdio.h>
#include <math.h>

const double eps = 1.0e-06;

double NewtonSqrt(double x) {
	double y = 1.0;
	double t = x;
	int nIter = 0;
	do {
		t = (y + x / y) / 2.0;
		if (fabs(t - y) < eps)
			break;

		y = t;
		nIter++;
	} while (nIter <= 20);

	return t;
}

void main() {
	double x = 0;
	while (x >= 0) {
		printf("Please enter a non-negative number: ");
		scanf("%lf", &x);
		printf("Square Root of x is %.12lf\n\n", NewtonSqrt(x));
	}
}