#include "GolombEncoder.h"

GolombEncoder::GolombEncoder(uint64_t k)
{
	this->k = k;
	//因为是Golomb Rice 
	m = pow(2, k);
}

void GolombEncoder::setBuffer(uint8_t *buffer, uint64_t len)
{
	bitStream.setBuffer(buffer, len);
}

void GolombEncoder::resetBuffer()
{
	bitStream.resetBuffer();
}

bool GolombEncoder::encode(uint64_t num)
{
	uint64_t len = 0;
	
	len = rice_golombEncode(num);

	// 空间足够，存放编码后的码元则返回true；否则返回false
	return bitStream.freeLength() >= len; 
}

bool GolombEncoder::close()
{
	return bitStream.flush();
}


// Golomb Rice
uint64_t GolombEncoder::rice_golombEncode(uint64_t num)
{
	//右移，相当于 q = num/ (x^k)
	uint64_t q = num >> k;
	//相当于 r = num - q * (x^k)
	uint64_t r = num & (m - 1);

	// 编码后码元的长度
	auto len = q + 1 + k;

	//不会判断缓存是否为满，直接向里面放，不足的话缓存到bit buffer中
	bitStream.putBit(1, q);
	bitStream.putBit(0);

	for (int i = 0; i < k; i++)
	{
		//判断r的最低一位，如果是1，那么putBit true,如果是0 ,那么putBit false;
		bitStream.putBit(static_cast<bool>(r & 0x01));
		//r右移一位。
		r >>= 1;
	}

	return len; // 返回编码后码元的长度
}

// Exp Golomb
uint64_t GolombEncoder::exp_golombEncode(uint64_t num)
{
	if (k != 0)
		num = num + pow(2, k) - 1;

	// 0阶exp-golom编码
	auto m = static_cast<int>(log(num + 1) / log(2)); // 前缀，也即是组号
	auto info = static_cast<int>(num + 1 - pow(2, m)); // 组内的偏移量

	auto len = 2 * m + 1; // 编码后码元的长度

						  // 写入m个0
	bitStream.putBit(0, m);
	// 写入1
	bitStream.putBit(1);

	// 写入info m-1 bits
	for (int i = 0; i < m; i++)
	{
		bitStream.putBit(static_cast<bool>(info & 0x01));
		info >>= 1;
	}

	return len;
}
