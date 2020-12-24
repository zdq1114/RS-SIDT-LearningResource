
#include <stdio.h>

void main() {
	int a;
	printf("Please input a integer: ");
	scanf("%d", &a);

	printf("a = ");
	if (a > 2) {
		for (int i = 2; i*i <= a; i++) {
			while (a % i == 0) {
				printf("%d X ", i);
				a = a / i;
			}
		}
		printf("%d\n\n", a);
	}
	else {
		printf("%d\n\n", a);
	}
}
