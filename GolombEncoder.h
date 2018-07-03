#pragma once

#include "BitStream.h"

class GolombEncoder
{
private:

	BitOutputStream bitStream;
	uint64_t m;
	uint64_t k;
public:

	//传递K值
	GolombEncoder(uint64_t k);

	//编码，编码前首先计算编码后码元的长度L。如果，缓存长度不足L bits。则将byte填充完后
	//，剩余的bit在BitBuffer中缓存，然后返回false，指出缓存不足以存放编码后的码元。
	bool encode(uint64_t num);

	//结束编码过程。将bit buffer中的数据flush到byte buffer中，然后填充64bits的0，
	//并判断totalCodeLength是否是8的倍数，如果不是，则继续补0.
	bool close();
	
	//buffer满了以后，处理满的buffer数据（写入文件、发送...），重置buffer，重新使用。
	void resetBuffer();
	//从外部设定buffer
	void setBuffer(uint8_t *buffer, uint64_t len);

	// 返回已经编码的字节byte长度，必须为8的倍数
	uint64_t getToalCodeLength()
	{
		return bitStream.getTotalCodeLength();
	}

private:
	//利用Golomb Rice 对 num进行编码，返回值是编码后的码元长度
	uint64_t rice_golombEncode(uint64_t num);
	
	//Exp Golomb
	uint64_t exp_golombEncode(uint64_t num);

};
