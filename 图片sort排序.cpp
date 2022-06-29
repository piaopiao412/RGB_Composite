

// 防止使用strcpy报warning
#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

/**
 * @brief 图像文件结构体
 */
struct ImageFile
{
	std::string file_name; /*!< 文件名 */
	int order_number; /*!< 文件序号, 取文件名 */
};

/**
* @brief 排序函数，从小到大
* @param img1  图片1
* @param img2  图片2
* @return 返回图片1的序号是否小于图片2
*	-- True 表示图片1小于图片2
*   -- False 表示图片1大于图片2
*/
bool SortByImageFiles(const ImageFile img1, const ImageFile img2)
{
	return img1.order_number < img2.order_number;
}

/**
* @brief 字符分割函数, this function adapt from: https://blog.csdn.net/mary19920410/article/details/77372828
* @param str  进行分割的字符串
* @param delim  分割符
* @return 返回分割后的字符vector
*/
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

	return res;
}

/**
* @brief 字符分割函数
* @param folder_path  文件夹路径
* @param file_format  文件类型
* @param file_names  ImageFile类型的vector，用于存放图片集合
*/
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
		cout << "----原顺序----" << endl;
		do {
			if ((fileInfo.attrib & _A_SUBDIR)) {
				continue;
			}
			else {
				temp_path.file_name = folder_path + fileInfo.name;
				split_result = split(fileInfo.name, ".");
				temp_path.order_number = atoi(split_result[0].c_str());
				file_names.push_back(temp_path);
				cout << temp_path.file_name << endl;
			}

		} while (_findnext(hFile, &fileInfo) == 0);
		std::sort(file_names.begin(), file_names.end(), SortByImageFiles);

		_findclose(hFile);
	}
}

int main()
{
	string a[250][4] = {};
	
	std::vector<ImageFile> file_names;
	GetImagePaths("E:\\temp\\", "*.bmp", file_names);

	//file_names[1]=
	cout << "----新顺序----"<<endl;
	for (int i = 0; i < 250; i++)
	{
		for (int j = 0; j < 4; j++)
		{
				a[i][j] = file_name[];
		}
	}


	//for (auto lin : file_names) {
	//	//cout << lin.file_name << endl;
	//	//cout << lin.order_number << endl;
	//	
	//}
	cout<< a[1][2];
	
	return 0;
}
