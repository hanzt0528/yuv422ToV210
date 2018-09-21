// Test_YUV422ToV210.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;


//this is a convert function,write done it work for both thinking:
//first yuv422 convert to v210,wish tell you about v210 format.
//second 8bit covert to 10bit.


int YUV422_8bit_To_V210_10bit(
	unsigned long ulWidth, unsigned long ulHeight,
	unsigned long ulSrcPitch, unsigned char* pSrc,
	unsigned long ulDstPitch, unsigned char* pDst
)
{
	unsigned char * pSrcLine = (unsigned char *)pSrc;
	unsigned char * pYUV422Line = (unsigned char *)pSrcLine;

	uint32_t *pV210Line = (uint32_t*)pDst;
	uint32_t *pV210 = pV210Line;

	for (; ulHeight > 0; ulHeight--)
	{
		pV210 = pV210Line;
		pYUV422Line = pSrcLine;
		for (int i = 0; i < ulWidth / 6; i++)
		{
			uint32_t Y1_32bit = *(pYUV422Line + 0);
			uint32_t U1_32bit = *(pYUV422Line + 1);
			uint32_t Y2_32bit = *(pYUV422Line + 2);
			uint32_t V1_32bit = *(pYUV422Line + 3);

			uint32_t Y3_32bit = *(pYUV422Line + 4);
			uint32_t U2_32bit = *(pYUV422Line + 5);
			uint32_t Y4_32bit = *(pYUV422Line + 6);
			uint32_t V2_32bit = *(pYUV422Line + 7);

			uint32_t Y5_32bit = *(pYUV422Line + 8);
			uint32_t U3_32bit = *(pYUV422Line + 9);
			uint32_t Y6_32bit = *(pYUV422Line + 10);
			uint32_t V3_32bit = *(pYUV422Line + 11);


			Y1_32bit = Y1_32bit << 2;
			U1_32bit = U1_32bit << 2;
			Y2_32bit = Y2_32bit << 2;
			V1_32bit = V1_32bit << 2;


			Y3_32bit = Y3_32bit << 2;
			U2_32bit = U2_32bit << 2;
			Y4_32bit = Y4_32bit << 2;
			V2_32bit = V2_32bit << 2;

			Y5_32bit = Y5_32bit << 2;
			U3_32bit = U3_32bit << 2;
			Y6_32bit = Y6_32bit << 2;
			V3_32bit = V3_32bit << 2;


			*pV210 = ((V1_32bit & 0x3ff) << 20) | ((Y1_32bit & 0x3ff) << 10) | (U1_32bit & 0x3ff);

			*(pV210 + 1) = ((Y3_32bit & 0x3ff) << 20) | ((U2_32bit & 0x3ff) << 10) | (Y2_32bit & 0x3ff);

			*(pV210 + 2) = ((U3_32bit & 0x3ff) << 20) | ((Y4_32bit & 0x3ff) << 10) | (V2_32bit & 0x3ff);

			*(pV210 + 3) = ((Y6_32bit & 0x3ff) << 20) | ((V3_32bit & 0x3ff) << 10) | (Y5_32bit & 0x3ff);


			pV210 += 4;
			pYUV422Line += 12;
		}
		pV210Line += (ulDstPitch / 4);
		pSrcLine += ulSrcPitch;

	}

	return 0;
}


int main()
{
	FILE *pfYUV422 = NULL;
	FILE *pfV210 = NULL;

	unsigned char *pYUV422_8bit = new unsigned char[3840 * 2160 * 2];
	unsigned char *pV210_10bit = new unsigned char[10240 * 2160];


	errno_t result = fopen_s(&pfYUV422, "UHDyuv422_8bit.yuv", "rb");

	if (result != 0)
	{
		printf("Open UHDyuv422_8bit.yuv file failed!");
		return 0;
	}
	
	

	fread(pYUV422_8bit, 1, 3840 * 2160*2, pfYUV422);

	fclose(pfYUV422);


	result = fopen_s(&pfV210, "V210_10bit.yuv", "wb");
	if (result != 0)
	{
		printf("Open V210_10bit.yuv file failed!");
		return 0;
	}

	//yuyv422-> v210
	YUV422_8bit_To_V210_10bit(3840, 2160, 3840 * 2, pYUV422_8bit, 10240, pV210_10bit);


	fwrite(pV210_10bit, 1, 10240 * 2160, pfV210);
	fclose(pfV210);

	printf("successful!");

    return 0;
}

