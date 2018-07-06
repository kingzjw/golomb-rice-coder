#include <iostream>
#include <random>
#include <fstream>
#include "zjw_golombCoder.h"
#include "zjw_RLGR.h"

using namespace std;

int main()
{

	int k =4;
	///////////////////////////////////////////////////////
	//
	// 从文件中读取测试数据
	//
	//////////////////////////////////////////////////////
	int length = 20;

	//uint64_t表示的是 unsigned long long。
	vector<uint64_t> numsList;
	vector<uint64_t> resList;


	ifstream codeIfs;
	codeIfs.open("code.txt");
	//random number generator
	std::random_device rd;
	uint64_t temp;
	for (int n = 0; n < length; ++n)
	{
		codeIfs >> temp;
		numsList.push_back(temp);
	}
	codeIfs.close();

	RLGR rlgr = RLGR(numsList,resList);
	rlgr.encode();
	rlgr.decode();



	system("pause");
	return 0;
}

void example1()
{
	int max = 20;
	//十万
	//int length = 100000;

	int length = 100;
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

	return ;
}
