// CoordTransformTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include "..\CoordTransform\CoordTransform.h"

#pragma comment(lib, "..\\Release\\CoordTransform.lib")

int main()
{
	CreateTransform(0, 0, 1, 0, 0, 1, 1, 1, 2, 1, 1, 2);
	double dbx, dby;
	Transform(1, 1, &dbx, &dby);
	printf_s("dbx: %lf, dby: %lf\n", dbx, dby);
	_tsystem(_T("pause"));
    return 0;
}

