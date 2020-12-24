
#include <stdio.h>
#include <memory.h>

void main() {
	int n = 0;
	printf("Please enter a positive integer: ");
	scanf_s("%d", &n);

	int* a = new int[n+1];
	memset(a, 0, (n + 1) * sizeof(int));

	for (int i = 2; i*i <= n+1; i++) {
		if (a[i] == 0) {
			for (int j = i + i; j < n+1; j += i) {
				a[j] = 1;
			}
		}
	}

	printf("The primes no greater than %d includes: \n", n);
	for (int i = 2; i < n+1; i++) {
		if (a[i] == 0) {
			printf("%d\n", i);
		}
	}

	if (a) delete[] a;
}