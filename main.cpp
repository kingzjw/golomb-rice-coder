#include <iostream>
#include <random>
#include <fstream>
#include "zjw_golombCoder.h"
#include "zjw_RLGR.h"

using namespace std;
int main()
{
	void GR_example();
	void RLGR_example();
	//测试数据
	void RLGR_example2();
	void RLGR_example3_readFromFile();

	RLGR_example3_readFromFile();

	system("pause");
	return 0;
}

void GR_example()
{

	int max = 20;

	int length = 10000;


	//uint64_t表示的是 unsigned long long。
	vector<uint64_t> numsList;
	vector<uint64_t> resList;


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
	for (int n = 0; n < length - 1; ++n)
	{
		numsList.push_back(rd() % max);
		codeOfs << numsList[n] << endl;
	}
	numsList.push_back(10);
	codeOfs << numsList[length - 1] << endl;

	codeOfs.close();


	///////////////////////////////////////////////////////
	//
	// encoder and decoder
	//
	//////////////////////////////////////////////////////
	int k = 8;
	GolombCoder coder = GolombCoder(&numsList, &resList,k);
	coder.encode();
	coder.decode();


	///////////////////////////////////////////////////////
	//
	// save the decoder info to file
	//
	//////////////////////////////////////////////////////

	ofstream of;
	of.open("decode.txt");

	for (int i = 0; i < resList.size(); i++)
	{
		of << resList[i] << endl;
	}
	of.close();

	return ;
}

void RLGR_example2()
{
	//文件中读取的元素个数
	int length = 24;
	//uint64_t表示的是 unsigned long long。
	vector<uint64_t> numsList;
	vector<uint64_t> resList;

	ifstream codeIfs;
	codeIfs.open("testGLGR_code.txt");
	//random number generator
	std::random_device rd;
	uint64_t temp;
	for (int n = 0; n < length; ++n)
	{
		codeIfs >> temp;
		numsList.push_back(temp);
	}
	codeIfs.close();

	RLGR rlgr = RLGR(&numsList, &resList);
	rlgr.encode();
	rlgr.decode();

	for (int i = 0; i < resList.size(); i++)
	{
		cout << resList[i] << endl;
	}
	system("pause");
	return ;
}

void RLGR_example3_readFromFile()
{
	int max = 20;

	int length = 1000;


	//uint64_t表示的是 unsigned long long。
	vector<uint64_t> numsList;
	vector<uint64_t> resList;


	///////////////////////////////////////////////////////
	//
	//  从文件中取得测试数据
	//
	//////////////////////////////////////////////////////

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


	///////////////////////////////////////////////////////
	//
	// encoder and decoder
	//
	//////////////////////////////////////////////////////
	RLGR rlgr = RLGR(&numsList, &resList);
	rlgr.encode();
	rlgr.decode();


	///////////////////////////////////////////////////////
	//
	// save the decoder info to file
	//
	//////////////////////////////////////////////////////

	ofstream of;
	of.open("decode.txt");

	for (int i = 0; i < resList.size(); i++)
	{
		of << resList[i] << endl;
	}
	of.close();

	return;
}

void RLGR_example()
{
	int max = 20;

	int length = 1000;


	//uint64_t表示的是 unsigned long long。
	vector<uint64_t> numsList;
	vector<uint64_t> resList;


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
	for (int n = 0; n < length-1; ++n)
	{
		if((rd() % 10)>=6)
			numsList.push_back(rd() % max);
		else
		{
			numsList.push_back(0);
		}
		codeOfs << numsList[n] << endl;
	}
	numsList.push_back(10);
	codeOfs << numsList[length - 1] << endl;

	codeOfs.close();


	///////////////////////////////////////////////////////
	//
	// encoder and decoder
	//
	//////////////////////////////////////////////////////
	RLGR rlgr = RLGR(&numsList, &resList);
	rlgr.encode();
	rlgr.decode();


	///////////////////////////////////////////////////////
	//
	// save the decoder info to file
	//
	//////////////////////////////////////////////////////

	ofstream of;
	of.open("decode.txt");

	for (int i = 0; i < resList.size(); i++)
	{
		of << resList[i] << endl;
	}
	of.close();

	return;
}
