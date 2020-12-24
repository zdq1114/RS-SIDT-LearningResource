
#include <stdio.h>

int GreatestCommonDivisor(unsigned int m, unsigned int n) {
	while (n != 0) {
		unsigned int t = m % n;
		m = n;
		n = t;
	}

	return m;
}

void main() {
	int x, y;
	printf("Enter a fraction: ");
	scanf_s("%d/%d", &x, &y);
	int gcd = GreatestCommonDivisor(x, y);
	printf("In lowest terms: %d/%d\n", x / gcd, y / gcd);
}