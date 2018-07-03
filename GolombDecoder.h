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

	//�������Kֵ
	GolombDecoder(uint64_t k);

	void setBuffer(uint8_t *buffer, int len);
	
	//���������ķ���ֵ��num
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

// ���� ��ÿ�ν���ǰ��Ҫcheck buffer��״̬�����ݲ�ͬ��״̬���������Ƿ����
BufferState GolombDecoder::decode(uint64_t& num)
{
	//�õ�����buffer��״̬
	auto state = bitStream.check();

	// buffer�������㹻�����н���
	if (state == BufferState::BUFFER_ENGOUGH)
	{

		num = rice_golombDecode();

	}
	else
	{
		//���ݿ��ܲ�����
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

	// group id ��0�ĸ�����ֱ��1
	b = bitStream.getBit();
	while (!b)
	{
		groupID++;
		b = bitStream.getBit();
	}

	// �������ڵ�ƫ����
	// ��ȡgroupID-1��bit
	std::bitset<64> bits;
	bits.reset();
	for (int i = 0; i < groupID; i++)
	{
		b = bitStream.getBit();
		bits.set(i, b);
	}

	uint64_t num = pow(2, groupID) - 1 + bits.to_ulong();
	//�ж��Ƿ���0��exp���������ֱ�ӷ��ؽ��
	if (k != 0)
		num = num - pow(2, k) + 1;
	return num;
}
