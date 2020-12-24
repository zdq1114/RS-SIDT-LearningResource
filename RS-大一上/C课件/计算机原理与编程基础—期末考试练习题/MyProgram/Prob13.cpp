
#include <stdio.h>
#include <math.h>

double IntegralFunc(double x) {
	return exp(-x*x);
}

double DefiniteIntegral(double a, double b, double eps, double(*Func)(double)) {
	double h = b - a;
	if (h <= 0) {
		printf("Error: the upper must be greater than the lower!\n");
		return 0;
	}

	double T1 = 0, T2 = 0;
	T1 = (Func(a) + Func(b))*h / 2.0;

	int iters = 0, n = 1;
	do {
		double* x = new double[n + 1];
		x[0] = a + 0.50*h;
		for (int i = 1; i < n + 1; i++) {
			x[i] = x[i-1] + h;
		}

		double sum = 0.0;
		for (int i = 0; i < n; i++) {
			sum += Func(x[i]);
		}
		T2 = (T1 + sum*h) / 2.0;

		if (fabs(T1 - T2) < eps)
			break;

		T1 = T2;
		h = h / 2;
		n = n * 2;
		iters++;
	} while (iters <= 30);

	return T2;
}

void main() {
	double a = 0;
	double b = 1;
	double eps = 1.0e-08;
	double s = DefiniteIntegral(a, b, eps, IntegralFunc);
	printf("The integral value is %.8lf\n", s);
}