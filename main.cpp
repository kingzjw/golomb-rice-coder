#include <iostream>
#include <random>
#include <fstream>
#include "zjw_golombCoder.h"


using namespace std;


int main()
{
	int max = 2000;
	//十万
	//int length = 100000;

	int length = 1000000;
	int k = 8;
	
	///////////////////////////////////////////////////////
	//
	// 随机生成测试数据
	//
	//////////////////////////////////////////////////////

	//uint64_t表示的是 unsigned long long。
	uint64_t *nums = new uint64_t[length];

	ofstream codeOfs;
	codeOfs.open("code.txt");
	//random number generator
	std::random_device rd;
	for (int n = 0; n < length; ++n)
	{
		nums[n] = rd() % max;
		codeOfs << nums[n] << endl;
	}
	codeOfs.close();

	///////////////////////////////////////////////////////
	//
	// 从文件读取生成测试数据
	//
	//////////////////////////////////////////////////////

	//uint64_t表示的是 unsigned long long。
	//uint64_t *nums = new uint64_t[length];

	//ofstream codeOfs;
	//codeOfs.open("code.txt");
	////random number generator
	//std::random_device rd;
	//for (int n = 0; n < length; ++n)
	//{
	//	nums[n] = rd() % max;
	//	codeOfs << nums[n] << endl;
	//}
	//codeOfs.close();



	///////////////////////////////////////////////////////
	//
	// encoder
	//
	//////////////////////////////////////////////////////
	
	GolombCoder coder = GolombCoder(k);
	
	coder.bitWriteFile->open();

	coder.encode(18);
	for (int i = 0; i < length; i++)
	{
		coder.encode(nums[i]);
	}

	coder.bitWriteFile->close();

	///////////////////////////////////////////////////////
	//
	// decoder version 1
	//
	//////////////////////////////////////////////////////
	coder.bitReadFile->open();

	ofstream of;
	of.open("decode.txt");

	uint64_t res = 0;
	coder.decode(res);
	for (int i = 0; i < length; i++)
	{
		coder.decode(res);
		of << res << endl;
	}
	of.close();
	coder.bitReadFile->close();

	///////////////////////////////////////////////////////
	//
	// decoder version 2
	//
	//////////////////////////////////////////////////////
	/*coder.bitReadFile->open();

	ofstream of;
	of.open("decode.txt");

	uint64_t res = 0;
	coder.decode(res);
	
	while(coder.decode(res))
		of << res << endl;
	
	of.close();
	coder.bitReadFile->close();*/


	return 0;
}
