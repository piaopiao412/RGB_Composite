#include <windows.h>
#include <wingdi.h>
#include <minwindef.h>
#include <iostream>
#include <fstream>
#include <string>

void save_IMG(PUCHAR data, BITMAPINFO* pBmpInfo, int nImgW, int nImgH, const char* filename);
//FILE* pfinTest = fopen("0.bmp", "rb");
//BITMAPFILEHEADER fileHeaderTest;
//BITMAPINFOHEADER infoHeaderTest;
//fread(&fileHeaderTest, sizeof(BITMAPFILEHEADER), 1, pfinTest);
//fread(&infoHeaderTest, sizeof(BITMAPINFOHEADER), 1, pfinTest);
//int heightTest, weightTest;
//heightTest = infoHeaderTest.biHeight;
//weightTest = infoHeaderTest.biWidth;
//PUCHAR pRImageTest = new unsigned char[infoHeaderTest.biSizeImage];
//fseek(pfinTest, 256 * sizeof(RGBQUAD), SEEK_CUR);
//fread(pRImageTest, infoHeaderTest.biSizeImage, 1, pfinTest);
//fclose(pfinTest);


int main()
{
	std::cout << "mmmm" << std::endl;
	int aReturn = -1;
	FILE* pfinR = fopen("R.bmp", "rb");
	BITMAPFILEHEADER fileHeaderR;
	BITMAPINFOHEADER infoHeaderR;
	aReturn = fread(&fileHeaderR, sizeof(BITMAPFILEHEADER), 1, pfinR);
	aReturn = fread(&infoHeaderR, sizeof(BITMAPINFOHEADER), 1, pfinR);
	unsigned int height, weight;
	height = infoHeaderR.biHeight;
	weight = infoHeaderR.biWidth;

	PUCHAR pRGBQuad = new unsigned char[256 * sizeof(RGBQUAD)];
	aReturn = fread(pRGBQuad, 256 * sizeof(RGBQUAD), 1, pfinR);




	PUCHAR pRImage = new unsigned char[infoHeaderR.biSizeImage];
	fseek(pfinR, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD), SEEK_SET);
	aReturn = fread(pRImage, infoHeaderR.biSizeImage, 1, pfinR);
	fclose(pfinR);

	std::cout << *(pRGBQuad + 5) << std::endl;
	PUCHAR pCOut = new unsigned char[4];
	*pCOut = '1';
	*(pCOut + 1) = 'a';
	std::cout << *(pCOut + 1) << std::endl;
	int bFH = sizeof(BITMAPFILEHEADER);
	int bIH = sizeof(BITMAPINFOHEADER);
	int BB = sizeof(BITMAPINFO);
	int rT = sizeof(RGBQUAD);


	FILE* pfinG = fopen("G.bmp", "rb");
	BITMAPFILEHEADER fileHeaderG;
	BITMAPINFOHEADER infoHeaderG;
	fread(&fileHeaderG, sizeof(BITMAPFILEHEADER), 1, pfinG);
	fread(&infoHeaderG, sizeof(BITMAPINFOHEADER), 1, pfinG);

	PUCHAR pGImage = new unsigned char[infoHeaderG.biSizeImage];
	fseek(pfinG, 256 * sizeof(RGBQUAD), SEEK_CUR);
	fread(pGImage, infoHeaderG.biSizeImage, 1, pfinG);
	fclose(pfinG);
	int all = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);


	FILE* pfinB = fopen("B.bmp", "rb");
	BITMAPFILEHEADER fileHeaderB;
	BITMAPINFOHEADER infoHeaderB;
	fread(&fileHeaderB, sizeof(BITMAPFILEHEADER), 1, pfinB);
	fread(&infoHeaderB, sizeof(BITMAPINFOHEADER), 1, pfinB);

	PUCHAR pBImage = new unsigned char[infoHeaderB.biSizeImage];
	fseek(pfinB, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD), SEEK_SET);
	fread(pBImage, infoHeaderB.biSizeImage, 1, pfinB);
	fclose(pfinB);





	BITMAPINFO* pBmpInfo;

	pBmpInfo = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD));
	pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmpInfo->bmiHeader.biWidth = weight;//nImgW
	pBmpInfo->bmiHeader.biHeight = height;//nImgH
	pBmpInfo->bmiHeader.biPlanes = 1;
	pBmpInfo->bmiHeader.biBitCount = 24;
	pBmpInfo->bmiHeader.biCompression = BI_RGB;
	pBmpInfo->bmiHeader.biSizeImage = weight * height * 3;//nImgW * nImgH
	pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	pBmpInfo->bmiHeader.biClrUsed = 256;//256
	pBmpInfo->bmiHeader.biClrImportant = 256;
	for (int index = 0; index < 1; index++)
	{
		pBmpInfo->bmiColors[index].rgbRed = (BYTE)index;
		pBmpInfo->bmiColors[index].rgbGreen = (BYTE)index;
		pBmpInfo->bmiColors[index].rgbBlue = (BYTE)index;
		pBmpInfo->bmiColors[index].rgbReserved = 0;
	}

	//开始合成
	PUCHAR pRGBImage = new unsigned char[3 * infoHeaderR.biSizeImage];
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < weight * 3; j += 3)
		{
			*(pRGBImage + i * weight * 3 + j) = *(pBImage + i * weight + j / 3);
			*(pRGBImage + i * weight * 3 + j + 1) = *(pGImage + i * weight + j / 3);
			*(pRGBImage + i * weight * 3 + j + 2) = *(pRImage + i * weight + j / 3);


		}
	}


	int iCamNum = 0;
	const char* filename = NULL;
	std::string sFilePath = "E:/01/ ";
	std::string sName = std::to_string(iCamNum);
	std::string sBMP = ".bmp";
	std::string sFileName = sFilePath + sName + sBMP;
	filename = sFileName.c_str();
	save_IMG(pRGBImage, pBmpInfo, weight, height, filename);
	delete[] pRImage;
	delete[] pGImage;
	delete[] pBImage;
	delete[] pRGBImage;
	return 0;
}


void save_IMG(PUCHAR data, BITMAPINFO* pBmpInfo, int nImgW, int nImgH, const char* filename)
{
	FILE* fp = fopen(filename, "wb");
	if (fp)
	{
		BITMAPFILEHEADER fheader = { 0 };
		fheader.bfType = 0x4d42;  //'MB','BM'
		fheader.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + pBmpInfo->bmiHeader.biSizeImage;
		fheader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
		fheader.bfReserved1 = 0;
		fheader.bfReserved2 = 0;

		int OK1 = 6;
		OK1 = fwrite(&fheader, 1, sizeof(fheader), fp);
		OK1 = fwrite(&(pBmpInfo->bmiHeader), 1, sizeof(BITMAPINFOHEADER), fp);
		OK1 = fwrite(data, 1, pBmpInfo->bmiHeader.biSizeImage, fp);
		fclose(fp);
	}
}