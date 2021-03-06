// CoordTransform.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

#pragma comment(lib, "opencv_core2411.lib")
#pragma comment(lib, "opencv_imgproc2411.lib")
#pragma comment(lib, "zlib.lib")

#define STORAGE_FILE_NAME		"Calibration.dat"

Mat g_transMat;

void WriteMat(Mat& src)
{
	char szPath[MAX_PATH] = { 0 };
	char szPathName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	char* p = strrchr(szPath, '\\');
	p[0] = '\0';
	sprintf(szPathName, "%s\\%s", szPath, STORAGE_FILE_NAME);

	int rows = src.rows;
	int cols = src.cols;

	fstream fs(szPathName, ios::out | ios::trunc);
	fs << "__MAT_B" << endl;
	fs << 100 << endl;
	fs << rows << endl;
	fs << 101 << endl;
	fs << cols << endl;
	fs << 102 << endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fs << src.at<double>(i, j) << endl;
		}
	}
	fs << "__MAT_E" << endl;
}


Mat ReadMat()
{
	char szPath[MAX_PATH] = { 0 };
	char szPathName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	char* p = strrchr(szPath, '\\');
	p[0] = '\0';
	sprintf(szPathName, "%s\\%s", szPath, STORAGE_FILE_NAME);

	Mat mat;
	string str;
	fstream fs(szPathName, ios::in);

	if (!fs)
	{
		OutputDebugStringA("=============================================================\n");
		OutputDebugStringA("CoordTransform warning! The calibration file does not exist!\n");
		OutputDebugStringA("=============================================================\n");
		return mat;
	}

	int rows, cols;
	if (fs.bad())
		return mat;

	while (!fs.eof())
	{
		fs >> str;
		if (!str.compare("__MAT_E"))
			break;
		if (!str.compare("100"))
		{
			fs >> str;
			rows = atoi(str.c_str());
			continue;
		}
		if (!str.compare("101"))
		{
			fs >> str;
			cols = atoi(str.c_str());
			continue;
		}
		if (!str.compare("102"))
		{
			Mat temp(rows, cols, CV_64F);
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					fs >> str;
					temp.at<double>(i, j) = atof(str.c_str());
				}
			}
			mat = temp;
			continue;
		}
	}
	return mat;
}

int __stdcall Transform(double srcX, double srcY, double* dstX, double* dstY)
{
	if (g_transMat.rows == 0) return -1;

	Mat matSrc(3, 1, CV_64F);

	matSrc.at<double>(0, 0) = srcX;
	matSrc.at<double>(1, 0) = srcY;
	matSrc.at<double>(2, 0) = 1;

	Mat matDst = g_transMat * matSrc;
	*dstX = matDst.at<double>(0, 0);
	*dstY = matDst.at<double>(1, 0);

	return 0;
}

int __stdcall CreateTransform(double srcX1, double srcY1, double srcX2, double srcY2, double srcX3, double srcY3, double dstX1, double dstY1, double dstX2, double dstY2, double dstX3, double dstY3)
{
	Point2f srcTri[3];
	Point2f dstTri[3];

	srcTri[0].x = srcX1;
	srcTri[0].y = srcY1;
	srcTri[1].x = srcX2;
	srcTri[1].y = srcY2;
	srcTri[2].x = srcX3;
	srcTri[2].y = srcY3;
	dstTri[0].x = dstX1;
	dstTri[0].y = dstY1;
	dstTri[1].x = dstX2;
	dstTri[1].y = dstY2;
	dstTri[2].x = dstX3;
	dstTri[2].y = dstY3;

	g_transMat = getAffineTransform(srcTri, dstTri);

	WriteMat(g_transMat);

	return 0;
}
