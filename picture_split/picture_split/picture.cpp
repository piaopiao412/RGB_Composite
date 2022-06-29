#include "picture.h"
#include <time.h>

#define ROWS	1250  // 存图行数

// 重组图片
bool SortByImageFiles(const ImageFile img1, const ImageFile img2)
{
	return img1.order_number < img2.order_number;
}

vector<string> split(const string& str, const string& delim)
{
	vector<string> res;
	if ("" == str) return res;
	char* strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char* p = strtok(strs, d);
	while (p) {
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);
	}
	//cout << str.length() << endl;

	return res;
}

void GetImagePaths(std::string folder_path, std::string file_format, std::vector<ImageFile>& file_names)
{
	intptr_t hFile = 0;
	_finddata_t fileInfo;
	std::string file_path = folder_path;
	file_path = file_path.append(file_format);
	hFile = _findfirst(file_path.c_str(), &fileInfo);
	ImageFile temp_path;
	vector<string> split_result;
	if (hFile != -1) {
		/*cout << "----原顺序----" << endl;*/
		do {
			if ((fileInfo.attrib & _A_SUBDIR)) {
				continue;
			}
			else {
				temp_path.file_name = folder_path + fileInfo.name;
				split_result = split(fileInfo.name, ".");
				temp_path.order_number = atoi(split_result[0].c_str());
				file_names.push_back(temp_path);
				//cout << temp_path.file_name << endl;
			}

		} while (_findnext(hFile, &fileInfo) == 0);
		std::sort(file_names.begin(), file_names.end(), SortByImageFiles);

		_findclose(hFile);
	}
}
IMAGEINFO GetWidHei(const char* filename)
{
	IMAGEINFO OUTFILE;

	int  ret = -1;
	// open r.bmp
	FILE* fpR = fopen(filename, "rb");

	BITMAPFILEHEADER fileheaderR;
	BITMAPINFOHEADER infoheaderR;

	ret = fread(&fileheaderR, sizeof(BITMAPFILEHEADER), 1, fpR);
	ret = fread(&infoheaderR, sizeof(BITMAPINFOHEADER), 1, fpR);

	OUTFILE.height = infoheaderR.biHeight;
	OUTFILE.width = infoheaderR.biWidth;

	//height = infoheaderR.biHeight;
	//weight = infoheaderR.biWidth;
	return OUTFILE;
}
// open R.bmp
int Getimage(const char * filename, unsigned char* pImage,int row)
{
	unsigned int height;
	unsigned int weight;

	int ret = -1;
	FILE* fpR = fopen(filename, "rb");

	BITMAPFILEHEADER fileheaderR;
	BITMAPINFOHEADER infoheaderR;

	ret = fread(&fileheaderR, sizeof(BITMAPFILEHEADER), 1, fpR);
	ret = fread(&infoheaderR, sizeof(BITMAPINFOHEADER), 1, fpR);

	height = infoheaderR.biHeight;
	weight = infoheaderR.biWidth;

	// rgb 
	unsigned char* pRGBQuard = new unsigned char[256 * sizeof(RGBQUAD)];
	ret = fread(pRGBQuard, 256 * sizeof(RGBQUAD), 1, fpR);

	delete[]pRGBQuard;
	// info
	//unsigned char pRimage[4096 * 1];
	fseek(fpR, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + weight * (height - row), SEEK_SET);
	ret = fread(pImage, weight * 1, 1, fpR);
 	fclose(fpR);
	return 0;
}
// save bmp
int SaveBmp(PUCHAR data, BITMAPINFOHEADER pBmpInfo, int nImgW, int nImgH, const char* filename)
{
	FILE* fp = fopen(filename, "wb");
	if (fp) 
	{	
		BITMAPFILEHEADER fheader;
		memset(&fheader, 0, sizeof(BITMAPFILEHEADER));
		RGBQUAD rgbquad[256];

		// mono8
		if (pBmpInfo.biBitCount == 8)
		{
			fheader.bfType = 0x4d42;  //'MB','BM'
			fheader.bfSize = (DWORD)sizeof(BITMAPINFOHEADER) + (DWORD)sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD) + nImgW * nImgH;
			fheader.bfOffBits = (DWORD)sizeof(BITMAPINFOHEADER) + (DWORD)sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD);
			fheader.bfReserved1 = 0;
			fheader.bfReserved2 = 0;

			for (int i = 0; i < 256; i++)
			{
				rgbquad[i].rgbBlue = i;
				rgbquad[i].rgbGreen = i;
				rgbquad[i].rgbRed = i;
				rgbquad[i].rgbReserved = 0;
			}
		}
		// RGB24
		else if (pBmpInfo.biBitCount == 24)
		{
			fheader.bfType = 0x4d42;  //'MB','BM'
			fheader.bfSize = (DWORD)sizeof(BITMAPINFOHEADER) + (DWORD)sizeof(BITMAPFILEHEADER) + nImgW * nImgH * 3;
			fheader.bfOffBits = (DWORD)sizeof(BITMAPINFOHEADER) + (DWORD)sizeof(BITMAPFILEHEADER);
			fheader.bfReserved1 = 0;
			fheader.bfReserved2 = 0;
		}


		int ret = -1;
		// fileheader
		ret = fwrite(&fheader, 1, sizeof(fheader), fp);
		// infoheader
		ret = fwrite(&(pBmpInfo), 1, sizeof(BITMAPINFOHEADER), fp);
		// MONO  rgnquad
		if (pBmpInfo.biBitCount == 8)/*filename.Write(&rgbquad, sizeof(RGBQUAD) * 256);*/
		{
			fwrite(&rgbquad, 1, sizeof(RGBQUAD) * 256, fp);
		}
		// data
		ret = fwrite(data, 1, pBmpInfo.biSizeImage, fp);
		fclose(fp);
	}
	return 0;
}


int main()
{	
	clock_t start_time, end_time;
	start_time = clock();
	std::vector<ImageFile> file_names;
	GetImagePaths("D:\\2022\\2022-06-20 17.40.08.510\\2022-06-20 17.40.08.510\\", "*.bmp", file_names);
	/*GetImagePaths("C:/Users/hq/Desktop/temp/", "*.bmp", file_names);*/
	
	cout << file_names[1].file_name<<endl;
	// 循环读取
	
	//vector<unsigned char*> mbgr[250];
	/*unsigned char* pRGBINFO = new unsigned char[3 * 4096 * ROWS];*/
	unsigned char * pRGBImage = new unsigned char[3 * 4096 * ROWS];

	int OutfileWid, OotfileHid;
	IMAGEINFO OUTFILE;
	OUTFILE = GetWidHei(file_names[1].file_name.c_str());
	OutfileWid = OUTFILE.width;	// width
	/*OotfileHid = OUTFILE.height;*/	// hight

	unsigned char* imagbuffR = new unsigned char[OutfileWid * 1];
	unsigned char* imagbuffG = new unsigned char[OutfileWid * 1];
	unsigned char* imagbuffB = new unsigned char[OutfileWid * 1];

	for (int i = 0; i < 4999; i += 4)
	{
			Getimage(file_names.at(i).file_name.c_str(), imagbuffR, 800);
			Getimage(file_names[i + 1].file_name.c_str(), imagbuffG,800);
			Getimage(file_names[i + 2].file_name.c_str(), imagbuffB,800);

		for (unsigned int j = 0; j < OutfileWid * 3; j += 3)
		{
			*(pRGBImage + i * OutfileWid * 3 / 4 + j) = *(imagbuffB + j / 3);
			*(pRGBImage + i * OutfileWid * 3 / 4 + j + 1) = *(imagbuffG +  j / 3);
			*(pRGBImage + i * OutfileWid * 3 / 4 + j + 2) = *(imagbuffR +  j / 3);
		}
	}

	BITMAPINFOHEADER pBmpInfo;
	memset(&pBmpInfo, 0, sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD));
	//pBmpInfo = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD));
	pBmpInfo.biSize = sizeof(BITMAPINFOHEADER);
	pBmpInfo.biWidth = OutfileWid;//nImgW
	pBmpInfo.biHeight = -ROWS;//nImgH	result_out   /*500*/
	pBmpInfo.biPlanes = 1;	// 	为目标设备说明颜色的平面数。其值总是为1。
	pBmpInfo.biBitCount = 24;	// 指定表示颜色时要用到的位数 1：黑白二色图; 4：16色图; 8：256色; 24：真彩色图
	pBmpInfo.biCompression = 0;		// BI_RGB  不压缩
	pBmpInfo.biSizeImage = OutfileWid * ROWS * 3;//nImgW * nImgH     //	指定实际的位图数据占用的字节数。biSizeImage = biWidth * biHeight
	pBmpInfo.biXPelsPerMeter = 0;	// 指定目标设备的水平分辨率。单位是每米的像素个数，
	pBmpInfo.biYPelsPerMeter = 0;	// 指定目标设备的竖直分辨率。单位是每米的像素个数，
	pBmpInfo.biClrUsed = 0;			//	指定本图象实际用到彩色表中的颜色索引数	若为0，则说明使用所有调色板项（数量为$\small{2^{biBitCount}}$)
	pBmpInfo.biClrImportant = 0;	// 指定本图象中重要的颜色数 若为0，则认为所有的颜色都是重要的


	// save image
	const char* filename = "out5.bmp";

	int ret = -1;
	ret = SaveBmp(pRGBImage, pBmpInfo, OutfileWid, ROWS, filename);
	//ret = SaveBmp(mbgr[0], pBmpInfo, 4096, ROWS, filename);
	if (ret == 0)
	{
		printf("fwrite success\n");
	}
	else
	{
		printf("fwrite error\n");
	}

	end_time = clock();
	cout << end_time - start_time << endl;
	return 0;
}