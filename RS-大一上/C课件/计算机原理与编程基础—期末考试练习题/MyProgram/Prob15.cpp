
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void GetFileTitle(const char* strPathName, char* strTitle, int nChar) {
	char* p = strrchr((char *)strPathName, '\\');
	strcpy_s(strTitle, nChar, p + 1);
	p = strchr(strTitle, '.');
	if (p) *p = '\0';
}

void main() {
	char strPathName[] = "d:\\Test\\debug\\x86\\test.exe";

	char strTitle[256];
	GetFileTitle(strPathName, strTitle, _countof(strTitle));
	printf("The title is %s\n", strTitle);
}