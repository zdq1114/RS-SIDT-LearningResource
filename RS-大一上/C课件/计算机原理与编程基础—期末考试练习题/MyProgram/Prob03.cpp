
#include <stdio.h>
#include <math.h>

void SolveQuadraticEqu(double a, double b, double c) {
	if (a != 0) {
		double delta = b*b - 4.0*a*c;
		double t = (delta >= 0) ? sqrt(delta) : sqrt(-delta);
		double re = -b / (2.0*a);
		double im = t / fabs(2.0*a);
		if (delta > 0) {
			printf("There are two real roots:\n");
			printf("x1 = %.6lf\n", re + im);
			printf("x2 = %.6lf\n", re - im);
		}
		else if (delta == 0) {
			printf("There are one multiple root:\n");
			printf("x = %.6lf\n", re);
		}
		else {
			printf("There are two imaginary roots:\n");
			printf("x1 = %.6lf + %.6lf i\n", re, im);
			printf("x2 = %.6lf - %.6lf i\n", re, im);
		}
	}
	else if (a == 0 && b != 0) {
		printf("The root is %.6lf\n", -c / b);
	}
	else if (a == 0 && b == 0 && c != 0) {
		printf("There is no root!\n");
	}
	else if (a == 0 && b == 0 && c == 0) {
		printf("There are infinitely many solutions!\n");
	}
}

void main() {
	printf("solve the quadratic equation with one unknown: \n");
	printf("a*x*x + b*x + c = 0\n\n");

	double a, b, c;
	printf("Please input a b c\n");
	scanf("%lf %lf %lf", &a, &b, &c);
	SolveQuadraticEqu(a, b, c);
}