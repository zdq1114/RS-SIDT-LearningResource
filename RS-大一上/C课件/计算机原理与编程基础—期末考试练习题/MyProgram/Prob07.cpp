
#include <stdio.h>
#include <string.h>

void ReverseString(char* strMsg, int nChar) {
	char* p1 = strMsg;
	char* p2 = strMsg + nChar - 1;
	while (p1 < p2) {
		char ch = *p1;
		*p1 = *p2;
		*p2 = ch;
		p1++;
		p2--;
	}
}

void main() {
	char strMsg[256];
	printf("Please enter a string: ");
	gets_s(strMsg);
	ReverseString(strMsg, strlen(strMsg));
	printf("The reverse string is: %s\n", strMsg);
}