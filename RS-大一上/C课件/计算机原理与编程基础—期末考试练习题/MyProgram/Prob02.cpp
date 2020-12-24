
#include <stdio.h>

void main() {
	printf("水仙花数包括：\n");
	for (int i = 100; i < 1000; i++) {
		int x = i / 100;
		int y = i/10%10;
		int z = i % 10;
		if (x*x*x + y*y*y + z*z*z == i) {
			printf("%3d\n", i);
		}
	}
}