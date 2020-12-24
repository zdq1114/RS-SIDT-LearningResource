
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char strCapital[]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char strLowercase[] = "abcdefghijklmnopqrstuvwxyz";

bool CaesarEncryption(char* strInfo, int nShift) {
	if (nShift < 1 || nShift > 25) 
		return false;

	char strCapTable[32] = { 0 }, strLowTable[32] = { 0 };
	memcpy(strCapTable, strCapital + nShift, (26 - nShift) * sizeof(char));
	memcpy(strLowTable, strLowercase + nShift, (26 - nShift) * sizeof(char));
	memcpy(strCapTable + 26 - nShift, strCapital, nShift * sizeof(char));
	memcpy(strLowTable + 26 - nShift, strLowercase, nShift * sizeof(char));

	char* p = strInfo;
	while (*p != '\0'){
		if (*p >= 'A' && *p <= 'Z') {
			int nCapId = (*p) - 'A';
			*p = strCapTable[nCapId];
		}
		else if (*p >= 'a' && *p <= 'z') {
			int nLowId = (*p) - 'a';
			*p = strLowTable[nLowId];
		}
		p++;
	}
}

void main() {
	printf("Test of Caesar Encryption\n");
	printf("Enter message to be encrypted: ");

	char strMsg[256];
	gets_s(strMsg);

	int nShift = 0;
	printf("Enter shift amount [1-25]: ");
	scanf("%d", &nShift);

	if (CaesarEncryption(strMsg, nShift)) {
		printf("Encrypted message: %s\n", strMsg);
	}
	else {
		printf("Error: invalid shift number!\n");
	}
}