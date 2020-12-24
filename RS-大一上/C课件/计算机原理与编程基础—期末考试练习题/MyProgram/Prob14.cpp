
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double CalcCorrelationCoef(double a[], double b[], int n) {
	double sa, sb, sa2, sb2, sab;
	sa = sb = sa2 = sb2 = sab = 0;
	for (int i = 0; i<n; i++) {
		sa += a[i]; sb += b[i]; sab += (a[i] * b[i]);
		sa2 += (a[i] * a[i]); sb2 += (b[i] * b[i]);
	}
	sa /= n; sb /= n; sab /= n; sa2 /= n; sb2 /= n;
	return (sab - sa*sb) / sqrt((sa2 - sa*sa)*(sb2 - sb*sb));
}

void main() {
	double x[] = { 1, 2, 3, 4, 5, 4, 3, 2, 1 };
	double* y = new double[_countof(x)];
	for (int i = 0; i < _countof(x); i++) {
		y[i] = 3 + 2 * x[i];  // 正相关, 相关系数为 1
		y[i] = y[i] * y[i];
//		y[i] = 3 - 2 * x[i];  // 负相关, 相关系数为-1
	}

//	y[4] = 99; // 加入一个噪声

	double crrc = CalcCorrelationCoef(x, y, _countof(x));
	printf("Correlation Coefficients of x and y is %lf\n", crrc);

	if (y) delete[] y;
}