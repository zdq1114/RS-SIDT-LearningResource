
#include <stdio.h>

void main() {
	int n = 0;
	for (int x = 0; x <= 3; x++) {
		for (int y = 1; y <= 5; y++) {
			for (int z = 0; z <= 6; z++) {
				if (x + y + z == 8) {
					n++;
					printf("Scheme %02d: Red(%d) + White(%d) + Black(%d)\n", n, x, y, z);
				}
			}
		}
	}
}