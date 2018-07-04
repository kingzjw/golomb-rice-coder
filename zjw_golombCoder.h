/*
* author: Jiawei Zhou
* time: 2018/7/3
* head file: Golomb coding��ʵ��(Golomb Rice  ��ʵ�� m��2��ָ��)
*/

#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <assert.h>
#include "zjw_bitFile.h"



class GolombCoder
{
private:
	string fileName;
	uint64_t m;
	uint64_t k;

public:
	BitReadFile  *bitReadFile;
	BitWriteFile *bitWriteFile;

public:
	//����Kֵ
	GolombCoder(uint64_t k, string fileName="golombRicd.gr");
	~GolombCoder();

	//��num����Golomb Rice�ı���
	void encode(uint64_t num);

	//���������ķ���ֵ��num,����false��ʾ�Լ��������ļ�ĩβ��
	bool decode(uint64_t& num);

	void setK(uint64_t k_);

private:

	//����Golomb Rice �� num���б��룬����ֵ�Ǳ�������Ԫ���ȡ�
	//use it must call  open close func in class bitWriteFile .
	void rice_golombEncode(uint64_t num);

	//Exp Golomb
	void exp_golombEncode(uint64_t num);

	//���ص���false�Լ����������ˡ�num�ǽ���������ֵ��
	//use it must call  open close func in class bitWriteFile .
	bool rice_golombDecode(uint64_t & num);

	//Exp Golomb
	uint64_t exp_golombDecode();
};