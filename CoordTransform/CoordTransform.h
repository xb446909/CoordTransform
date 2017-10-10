#pragma once

#ifndef COORDTRANSFORM_EXPORTS
__declspec(dllimport) int __stdcall CreateTransform(double srcX1, double srcY1, double srcX2, double srcY2, double srcX3, double srcY3, double dstX1, double dstY1, double dstX2, double dstY2, double dstX3, double dstY3);
__declspec(dllimport) int __stdcall Transform(double srcX, double srcY, double* dstX, double* dstY);
#endif // !COORDTRANSFORM_EXPORTS