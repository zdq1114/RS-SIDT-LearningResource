
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int SearchKeywordInString(const char* strLine, const char* strWord) {
	int nFinds = 0;
	int nChar = strlen(strWord);
	char* str = strstr((char *)strLine, strWord);
	while (str) {
		nFinds++;

		int nLasts = strlen(str);
		if (nLasts > nChar) str += nChar;
		else break;

		str = strstr(str, strWord);
	}

	return nFinds;
}

int SearchKeywordInFile(const char* strFile, const char* strWord) {
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, strFile, "rt");
	if (!fp) {
		printf("Error: Can not open file %s!\n", strFile);
		return 0;
	}

	int nFinds = 0;
	while (!feof(fp)) {
		char strLine[256] = { 0 };
		fgets(strLine, _countof(strLine), fp);
		nFinds += SearchKeywordInString(strLine, strWord);
	}

	fclose(fp);

	return nFinds;
}

void main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Error: The run-time parameters is necessary!\n");
		return;
	}

	printf("Search in file %s\n", argv[1]);

	char strKeywords[256];
	printf("Enter keyword: ");
	gets_s(strKeywords);
	int nFinds = SearchKeywordInFile(argv[1], strKeywords);
	printf("Total %d finds!\n", nFinds);
}