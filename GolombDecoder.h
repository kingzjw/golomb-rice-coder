#pragma once

#include "BitStream.h"
#include  "GolombEncoder.h"


class GolombDecoder
{
private:
	BitInputStream bitStream;
	uint64_t k;
	uint64_t m;

private:
	uint64_t rice_golombDecode();
	uint64_t exp_golombDecode();

public:

	//输入的是K值
	GolombDecoder(uint64_t k);

	void setBuffer(uint8_t *buffer, int len);
	
	//解析出来的返回值：num
	BufferState decode(uint64_t& num);


};


GolombDecoder::GolombDecoder(uint64_t k)
{

	this->k = k;
	m = pow(k, 2);

}

void GolombDecoder::setBuffer(uint8_t *buffer, int len)
{
	bitStream.setBuffer(buffer, len);
}

// 解码 在每次解码前需要check buffer的状态，根据不同的状态决定解码是否继续
BufferState GolombDecoder::decode(uint64_t& num)
{
	//得到现在buffer的状态
	auto state = bitStream.check();

	// buffer中数据足够，进行解码
	if (state == BufferState::BUFFER_ENGOUGH)
	{

		num = rice_golombDecode();

	}
	else
	{
		//数据可能不够了
		int i = 234234;
	}

	return state;
}


// Rice Golomb
uint64_t GolombDecoder::rice_golombDecode()
{
	bool b;
	uint64_t unary = 0;
	b = bitStream.getBit();
	while (b)
	{
		unary++;
		b = bitStream.getBit();
	}

	std::bitset<64> bits;
	bits.reset();
	for (int i = 0; i < k; i++)
	{
		b = bitStream.getBit();
		bits.set(i, b);
	}

	uint64_t num = unary * m + bits.to_ulong();

	return num;
}

// Exp Golomb
uint64_t GolombDecoder::exp_golombDecode()
{
	bool b;
	uint64_t groupID = 0;

	// group id 是0的个数，直到1
	b = bitStream.getBit();
	while (!b)
	{
		groupID++;
		b = bitStream.getBit();
	}

	// 解码组内的偏移量
	// 读取groupID-1个bit
	std::bitset<64> bits;
	bits.reset();
	for (int i = 0; i < groupID; i++)
	{
		b = bitStream.getBit();
		bits.set(i, b);
	}

	uint64_t num = pow(2, groupID) - 1 + bits.to_ulong();
	//判断是否是0阶exp，如果是则直接返回结果
	if (k != 0)
		num = num - pow(2, k) + 1;
	return num;
}
