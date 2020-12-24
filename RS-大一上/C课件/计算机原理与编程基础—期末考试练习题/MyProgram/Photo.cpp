// Photo.cpp: implementation of the CPhoto class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Photo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define _DEBUG_ATXGPC_DLL
#define _DEBUG_SPVZIMAGE
#define _DEBUG_WUDPGIMAGE
#define _DEBUG_PRE_PROCESS
#define _DEBUG_F_MATCH_DLL
#endif

#include "WuDPGImage.h"
#include "FMatch.h"
#include "AtxGPC.h"
#include "PreProcess.h"
#include "CBERSImage.h"

const int FEATURE_GRID_SIZE = 8;//16;

double PI = 3.1415926535897932384626433832795;
double LIMITANGLE = PI/6;
double DblPI = 2.0*PI;

bool bAntiClockWise = true;

inline double NrmlRadian(double a){
	while (a < 0){
		a += DblPI;
	}
	while (a > DblPI){
		a -= DblPI;
	}
	return a;
}

void GetWallisImagePathName(const char* lpszPathName, char* strWallisImage){
	char strPath[_MAX_FNAME];
	char strName[_MAX_FNAME];
	AtxGetFilePath(lpszPathName, strPath);
	AtxGetFileTitle(lpszPathName, strName);

	sprintf(strWallisImage, "%s\\%s_wallis.tif", strPath, strName);
}

void GetWallisImagePathName(const char* lpszPathName, char* strWallisImage, const char* strTmpPath){
	char strName[_MAX_FNAME];
	AtxGetFileTitle(lpszPathName, strName);
	sprintf(strWallisImage, "%s\\%s_wallis.tif", strTmpPath, strName);
}

void SubExtractHarris(const char* lpszPathName, const char* strPhotoID, int nZoom, const char* lpszTmpPath){
	char strPydImgPath[_MAX_FNAME];
	GetImagePyramidPathName(lpszPathName, nZoom, strPydImgPath, lpszTmpPath);
	
	if (nZoom >= 4){
		int nFpts = 0;
		ExtractFeaturePts(strPydImgPath, &nFpts);
	}

	char strPath[_MAX_FNAME];
	char strName[_MAX_FNAME];
	AtxGetFilePath(strPydImgPath, strPath);
	AtxGetFileTitle(strPydImgPath, strName);

	char strFile[_MAX_FNAME];
	sprintf(strFile, "%s\\%s-harris.txt", strPath, strName);
	if (_access(strFile, 0) == 0) return;

	int grid_size = FEATURE_GRID_SIZE;
	PhotoPt *pHarris = NULL;
	int nHarris = 0;
	ExtractHarrisPoint(strPydImgPath, grid_size, &pHarris, &nHarris);
	
	char strText[32];
	for (int i=0; i<nHarris; i++){
		sprintf(strText, "%0.5d", i+1);
		sprintf(pHarris[i].ID, "%s%s", strPhotoID, strText);
	}

	SaveHarrisPt2File(strFile, pHarris, nHarris);
	
	ReleaseHarrisPoint(pHarris);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhoto::CPhoto(){
	strcpy(m_strPathName, "");
	strcpy(m_strTempPath, "");

	m_nPhotoID = 0;
	m_nCols = 0;
	m_nRows = 0;

	memset(&m_iop, 0, sizeof(IOP));
	memset(&m_eop, 0, sizeof(EOP));
	RotateMatrix(m_eop.p, m_eop.w, m_eop.k, m_eop.r);

	m_TieArray.SetSize(512, 512);

	m_bOri = false;
}

CPhoto::~CPhoto(){
}

void CPhoto::SetPathName(const char* lpszPathName){
	CWuDPGImage* pImg = new CWuDPGImage;
	if (!pImg->Open(lpszPathName))
		return;

	m_nCols = pImg->GetCols();
	m_nRows = pImg->GetRows();

	pImg->Close();
	delete pImg;

	strcpy(m_strPathName, lpszPathName);
}

int CPhoto::GetInitZoomRate(){
	CWuDPGImage* pImg = new CWuDPGImage;
	if (!pImg->Open(m_strPathName))
		return 1;

	int nCols = pImg->GetCols();
	int nRows = pImg->GetRows();

	pImg->Close();
	delete pImg;

	int nSize = min(nCols, nRows);
	int nRatio = nSize/512 + 1;
//	if (nRatio > 5) nRatio = 5;
	return nRatio;
}

void CPhoto::PreProcess(int* pZoom, int nZoom){
	CreateHierachical(m_strPathName, pZoom, nZoom, m_strTempPath);

	char strWallisFile[256];
	GetWallisImagePathName(m_strPathName, strWallisFile, m_strTempPath);

	if (_access(strWallisFile, 0) != 0){
		for (int i=0; i<nZoom; i++){
			char strPyramidImgFile[256];
			GetImagePyramidPathName(m_strPathName, pZoom[i], strPyramidImgFile, m_strTempPath);

			char strWallisImgFile[256];
			GetWallisImagePathName(strPyramidImgFile, strWallisImgFile, m_strTempPath);

			DPGFileAdaptiveWallisFiltering(strPyramidImgFile, strWallisImgFile, 127.0f, 80.0f, 0.75f, 0.60f, 8, 8, 8, 1);
			
			if (pZoom[i] != 1){
				CopyFile(strWallisImgFile, strPyramidImgFile, FALSE);
				DeleteFile(strWallisImgFile);
			}
		}
	}

	char strPhotoID[16];
	sprintf(strPhotoID, "%d", m_nPhotoID);
	SubExtractHarris(m_strPathName, strPhotoID, pZoom[nZoom-1], m_strTempPath);

	for (int i=nZoom-2; i>=0; i--){
		SubExtractHarris(m_strPathName, pZoom[i], pZoom[i+1], m_strTempPath);
	}

/*	for (int i=nZoom-1; i>=0; i--){
		SubExtractHarris(m_strPathName, strPhotoID, pZoom[i], m_strTempPath);
	}*/
}

void CPhoto::PreProcess4InitAT(){
	int pZoom[] = {1, 1};
	pZoom[1] = GetInitZoomRate();
	int nZoom = sizeof(pZoom)/sizeof(int);

	char strPyramidImgFile[256];
	GetImagePyramidPathName(m_strPathName, pZoom[nZoom-1], strPyramidImgFile, m_strTempPath);
	if (access(strPyramidImgFile, 0) == 0) return;

	CreateHierachical(m_strPathName, pZoom, nZoom, m_strTempPath);

	char strWallisImgFile[256];
	GetImagePyramidPathName(m_strPathName, pZoom[nZoom-1], strPyramidImgFile, m_strTempPath);
	GetWallisImagePathName(strPyramidImgFile, strWallisImgFile);
//	DPGFileAdaptiveWallisFiltering(strPyramidImgFile, strWallisImgFile, 127.0f, 80.0f, 0.75f, 0.60f, 15, 15, 16, 1);
	DPGFileAdaptiveWallisFiltering(strPyramidImgFile, strWallisImgFile, 127.0f, 80.0f, 0.75f, 0.60f, 8, 8, 8, 1);
	CopyFile(strWallisImgFile, strPyramidImgFile, FALSE);
	DeleteFile(strWallisImgFile);

	char strPhotoID[16];
	sprintf(strPhotoID, "%d", m_nPhotoID);
	SubExtractHarris(m_strPathName, strPhotoID, pZoom[nZoom-1], m_strTempPath);
}

void CPhoto::AddPoint(int ID, double x, double y){
	ImgPt* p = m_TieArray.GetData();
	int n = m_TieArray.GetSize();
	for (int i=0; i<n ;i++){
		if (p[i].ID == ID){
			return;
		}
	}

	ImgPt pt;
	pt.ID = ID;
	pt.x  = x;
	pt.y  = y;
	pt.rx = 0;
	pt.ry = 0;
	m_TieArray.Add(pt);
}

void CPhoto::SetResiduals(int ID, double rx, double ry){
	ImgPt* pTie = m_TieArray.GetData();
	int nTie = m_TieArray.GetSize();

	for (int i=0; i<nTie; i++){
		if (pTie[i].ID == ID){
			pTie[i].rx = rx;
			pTie[i].ry = ry;
			return;
		}
	}
}

void CPhoto::DeleteAbnormalPoint(){
	ImgPt* pTie = m_TieArray.GetData();
	int nTie = m_TieArray.GetSize();

	ImgPt* pTmp = new ImgPt[nTie];
	memcpy(pTmp, pTie, nTie*sizeof(ImgPt));

	m_TieArray.RemoveAll();
	m_TieArray.SetSize(512, 512);

	for (int i=0; i<nTie; i++){
		if (fabs(pTmp[i].rx) < LIMITERR && fabs(pTmp[i].ry) < LIMITERR){
			m_TieArray.Add(pTmp[i]);
		}
	}

	if (pTmp) delete[] pTmp;
}

bool CPhoto::IsNeighbor(CPhoto* pImg){
	if (!pImg)          return false;
	if (pImg == this)   return false;
	if (!m_bOri)        return false;
	if (!pImg->IsOri()) return false;

	// 对于旋转摄影设置了摄影间角度的上限（2*30 = 60度）
	EOP* pEOP1 = &m_eop;
	EOP* pEOP2 = pImg->GetEOP();

	double* r1 = pEOP1->r;
	double* r2 = pEOP2->r;
	double f1 = m_iop.f*PIXELSIZE;
	double f2 = pImg->GetFocalLength();

	double u1 = -r1[2]*f1, u2 = -r2[2]*f2;
	double v1 = -r1[5]*f1, v2 = -r2[5]*f2;
	double w1 = -r1[8]*f1, w2 = -r2[8]*f2;

	double a = acos((u1*u2 + v1*v2 + w1*w2)/sqrt(u1*u1 + v1*v1 + w1*w1)/sqrt(u2*u2 + v2*v2 + w2*w2));
	if (a >= LIMITANGLE) return false;

	return true;
}

void CPhoto::IxyToPxy(double i, double j, double* x, double* y){
	*x = (i - m_iop.u0)*PIXELSIZE;
	*y = (j - m_iop.v0)*PIXELSIZE;
}

void CPhoto::PxyToIxy(double x, double y, double* i, double* j){
	*i = x/PIXELSIZE + m_iop.u0;
	*j = y/PIXELSIZE + m_iop.v0;
}

void CPhoto::GrdToIxy(double gx, double gy, double gz, double* sx, double* sy){
	double dx = gx - m_eop.x;
	double dy = gy - m_eop.y;
	double dz = gz - m_eop.z;
	double u = m_eop.r[0]*dx + m_eop.r[3]*dy + m_eop.r[6]*dz;
	double v = m_eop.r[1]*dx + m_eop.r[4]*dy + m_eop.r[7]*dz;
	double w = m_eop.r[2]*dx + m_eop.r[5]*dy + m_eop.r[8]*dz;
	double f = PIXELSIZE*m_iop.f;
	double px = -f*u/w;
	double py = -f*v/w;

	PxyToIxy(px, py, sx, sy);
}

void CPhoto::IxyToGxy(double sx, double sy, double h, double* gx, double* gy){
	double px, py;
	IxyToPxy(sx, sy, &px, &py);

	double* r = m_eop.r;
	double f = PIXELSIZE*m_iop.f;
	double l = r[0]*px + r[1]*py - r[2]*f;
	double m = r[3]*px + r[4]*py - r[5]*f;
	double n = r[6]*px + r[7]*py - r[8]*f;

	*gx = (h - m_eop.z)*l/n + m_eop.x;
	*gy = (h - m_eop.z)*m/n + m_eop.y;
}

bool CPhoto::ReadGrayImageData(int nZoom, int* nCols, int* nRows, unsigned char** data){
	if (*data) delete[] (*data);
	(*data) = NULL;
	(*nCols) = 0;
	(*nRows) = 0;

	char strPydImgPath[_MAX_FNAME];
	GetImagePyramidPathName(m_strPathName, nZoom, strPydImgPath, NULL);

	CWuDPGImage img;
	if (!img.Open(strPydImgPath))
		return false;

	int cols = img.GetCols();
	int rows = img.GetRows();
	unsigned char* pData = new unsigned char[cols*rows];
	memset(pData, 0, cols*rows*sizeof(unsigned char));

	bool bSucceed = (img.Read(pData, 1, 0, 0, rows, cols)) ? true : false;
	if (bSucceed){
		(*data) = pData;
		(*nCols) = cols;
		(*nRows) = rows;
	}

	return bSucceed;
}
