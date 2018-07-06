#include <iostream>
#include <random>
#include <fstream>
#include "zjw_golombCoder.h"
#include "zjw_RLGR.h"

using namespace std;
int main()
{
	void RLGR_example();
	void RLGR_example2();

	RLGR_example2();
	
}

void GR_example()
{
	int max = 20;
	//ʮ��
	//int length = 100000;

	int length = 100;
	int k = 8;

	///////////////////////////////////////////////////////
	//
	// ������ɲ�������
	//
	//////////////////////////////////////////////////////

	//uint64_t��ʾ���� unsigned long long��
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

void RLGR_example2()
{
	//�ļ��ж�ȡ��Ԫ�ظ���
	int length = 24;
	//uint64_t��ʾ���� unsigned long long��
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

void RLGR_example()
{
	int max = 20;

	int length = 1000;


	//uint64_t��ʾ���� unsigned long long��
	vector<uint64_t> numsList;
	vector<uint64_t> resList;


	///////////////////////////////////////////////////////
	//
	// ������ɲ�������
	//
	//////////////////////////////////////////////////////

	//uint64_t��ʾ���� unsigned long long��
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
	assert(resList.size() == numsList.size());

	ofstream of;
	of.open("decode.txt");

	for (int i = 0; i < resList.size(); i++)
	{
		of << resList[i] << endl;
	}
	of.close();

	return;
}
