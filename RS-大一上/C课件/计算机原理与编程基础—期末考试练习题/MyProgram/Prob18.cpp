
#include <stdio.h>

typedef struct tagGCP {
	int ID;
	double x, y, z;
}GCP;

bool ReadGrdCtrlPtFile(const char* strFile, GCP** pGCP, int* nGCP) {
	if (*pGCP) delete[](*pGCP);
	(*pGCP) = nullptr;
	(*nGCP) = 0;

	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, strFile, "rt");
	if (!fp) {
		printf("Error: can not open file %s!\n", strFile);
		return false;
	}

	fscanf_s(fp, "%d\n", nGCP);
	if (*nGCP) {
		(*pGCP) = new GCP[*nGCP];
		for (int i = 0; i < *nGCP; i++) {
			fscanf_s(fp, "%d %lf %lf %lf\n", &(*pGCP)[i].ID,
				&(*pGCP)[i].x, &(*pGCP)[i].y, &(*pGCP)[i].z);
		}
	}

	fclose(fp);
	return true;
}

GCP* ReadGrdCtrlPtFile(const char* strFile, int* nGCP) {
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, strFile, "rt");
	if (!fp) {
		printf("Error: can not open file %s!\n", strFile);
		return nullptr;
	}

	GCP* pGCP = nullptr;
	fscanf_s(fp, "%d\n", nGCP);
	if ((*nGCP) > 0) {
		pGCP = new GCP[*nGCP];
		for (int i = 0; i < *nGCP; i++) {
			fscanf_s(fp, "%d %lf %lf %lf\n", &pGCP[i].ID, &pGCP[i].x,
				&pGCP[i].y, &pGCP[i].z);
		}
	}

	fclose(fp);
	return pGCP;
}

bool WriteGCPAsBinFile(const char* strFile, GCP* pGCP, int nGCP) {
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, strFile, "wb");
	if (!fp) {
		printf("Error: can not open file %s!\n", strFile);
		return false;
	}

	fwrite(&nGCP, sizeof(int), 1, fp);
	fwrite(pGCP, sizeof(GCP), nGCP, fp);

	fclose(fp);
	return true;
}

void main() {
	char strTxtFile[] = "f:\\MyProgram\\gcp.txt";
	char strBinFile[] = "f:\\MyProgram\\gcp.bin";

	GCP* pGCP = nullptr;
	int nGCP = 0;
	if (ReadGrdCtrlPtFile(strTxtFile, &pGCP, &nGCP)) {
		WriteGCPAsBinFile(strBinFile, pGCP, nGCP);
	}

	if (pGCP) delete[] pGCP;
}