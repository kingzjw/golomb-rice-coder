#include <iostream>
#include <random>
#include <fstream>
#include "zjw_golombCoder.h"


using namespace std;

static const int length = 50000;
static const int  max = 20;

int main()
{
	uint8_t buffer[1024];
	int k = 2;

	
	///////////////////////////////////////////////////////
	//
	// 随机生成测试数据
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
	

	GolombCoder coder= GolombCoder(k);
	
	coder.bitWriteFile->open();
	coder.encode(18);

	coder.bitWriteFile->close();

	coder.bitReadFile->open();
	uint64_t res = 0;
	coder.decode(res);
	cout << "res:" << res << endl;
	coder.bitReadFile->close();

	
	
	return 0;
}

void test1()
{
	/*
	uint8_t buffer[1024];
	int k = 2;


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


	////////////////////////////////////////////////////
	//
	// Encode
	//
	///////////////////////////////////////////////////
	GolombEncoder encoder(k);
	encoder.setBuffer(buffer, 1024);

	ofstream ofs;
	ofs.open("golomb.gl", ios::binary);

	for (int i = 0; i < length; i++)
	{
	auto b = encoder.encode(nums[i]);

	//剩下的空间不够写的，那么就需要先保存到文件中，再做存储
	if (!b)
	{
	cout << "Lack of buffer space,write the data to file" << endl;
	cout << "reset buffer" << endl;
	ofs.write((const char*)buffer, encoder.getToalCodeLength());

	encoder.resetBuffer();
	break;
	}
	}
	encoder.close();

	ofs.write((const char*)buffer, encoder.getToalCodeLength());

	ofs.close();

	cout << "Golomb finished coding" << endl;

	////////////////////////////////////////////////////////////////////
	//
	// Decode
	//
	////////////////////////////////////////////////////////////////////

	ifstream ifs;
	ifs.open("golomb.gl", ios::binary);

	memset(buffer, 0, 1024);

	ifs.read((char*)buffer, 664);

	ofstream encodeOfs;
	encodeOfs.open("encode.txt");

	GolombDecoder decoder(k);
	decoder.setBuffer(buffer, 1024);
	uint64_t num;
	auto state = decoder.decode(num);

	int index = 0;
	while (state != BufferState::BUFFER_END_SYMBOL)
	{
	encodeOfs << num << endl;
	state = decoder.decode(num);

	index++;
	cout << "index: " << index << endl;
	}

	ifs.close();
	encodeOfs.close();

	cout << "decode finished" << endl;

	// 2阶exp_golomb

	GolombEncoder expGolomb(2);
	expGolomb.setBuffer(buffer, 1024);

	expGolomb.encode(5);
	expGolomb.encode(9);
	expGolomb.encode(16);
	expGolomb.encode(27);

	// 结束编码
	expGolomb.close();

	//解码
	GolombDecoder expDecoder(2);
	expDecoder.setBuffer(buffer, 1024);

	uint64_t a;
	expDecoder.decode(a);
	cout << a << endl;

	expDecoder.decode(a);
	cout << a << endl;

	expDecoder.decode(a);
	cout << a << endl;

	expDecoder.decode(a);
	cout << a << endl;

	getchar();
	return 0;

	*/
}