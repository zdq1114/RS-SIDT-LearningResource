
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
	_mkdir(strPath);
}

void main() {
	CreateDirectory("D:\\Test\\Level1\\Level2\\Level3");
}