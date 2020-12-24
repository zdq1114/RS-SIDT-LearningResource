
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <io.h>

void CreateDirectory(const char* strPath) {
	// split the input directory by the last '\\'
	char strTemp[256];
	strcpy_s(strTemp, _countof(strTemp), strPath);
	char* p = strrchr(strTemp, '\\');

	// terminate the recursion
	if (!p) return;

	// and get the parent directory name
	*p = '\0';

	// recurrent iterated to create parent directories
	CreateDirectory(strTemp);
	if (_access(strPath, 0) != 0) _mkdir(strPath);
}

void CreateDirectoryEx(const char* strPath) {
	char* p = strchr((char *)strPath, '\\');
	while (p) {
		char strTemp[256] = { 0 };
		memcpy(strTemp, strPath, (p - strPath) * sizeof(char));
		if (_access(strTemp, 0) != 0) _mkdir(strTemp);
		p = strchr(p + 1, '\\');
	}
	if (_access(strPath, 0) != 0) _mkdir(strPath);
}

// 函数CreateDirectory  采用递归方法实现
// 函数CreateDirectoryEx采用循环方法实现
void main() {
	char strPath[] = "D:\\Test\\Level1\\Level2\\Level3";
//	CreateDirectory(strPath);
	CreateDirectoryEx(strPath);
}