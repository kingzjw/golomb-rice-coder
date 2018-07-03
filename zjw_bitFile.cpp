#include "zjw_bitFile.h"


ReadBuffer::ReadBuffer(fstream * filePtr)
{
	assert(bitBufferLength % 8 == 0);


	//file
	this->filePtr = filePtr;

	//bit 
    bitBuffer.reset();
    bitPos = 0;

	//byte
	byteBuffer = new uint8_t[byteBufferLength];
	bytePos = -1;
	validBitsNum = -1;
	bytePos = -1;
	validByteNum = -1;

}

ReadBuffer::~ReadBuffer()
{
	delete byteBuffer;
}

bool ReadBuffer::getBit(bool &bit)
{
	if (bitPos < bitBufferLength)
	{
		bit = bitBuffer[bitPos];
		bitPos++;
		return true;
	}
	else
	{
		//bitbuffer中的数据都被用完，需要从bytebuffer中获取数据。
		if (getNextBytesFromByteBuffer()>0)
		{
			bit = bitBuffer[bitPos];
			return true;
		}
		else
		{
			//bytebuffer中没有足够的数据了
			bitPos = -1;
			return false;
		}
	}
}

bool ReadBuffer::getByte(uint8_t & byte) {

	if (bytePos < validByteNum)
	{
		byte = byteBuffer[bytePos];
		return true;
	}
	else
	{
		if (getBytesFromFile()>0)
		{
			//byte buffer中的数据不够，需要读取数据。
			byte = byteBuffer[bytePos];
			return true;
		}
		else
		{
			//没有足够的数据了
			bytePos = -1;
			return false;
		}
	}
}

//只保证bitBufferLength不大于64的时候是对的,调用这个接口之前保证，bitbuffer中的数据已经用完。 
uint8_t ReadBuffer::getNextBytesFromByteBuffer()
{
	assert(bitPos >= validBitsNum);

	//init bitBuffer 都为0
	bitBuffer.reset();
	bitPos = 0;
	
	//从byteBuffer中读取若干个有效数据。
	for (int b_it = 0; b_it < bitBufferLength / 8; b_it++)
	{
		uint8_t  byte;
		if (getByte(byte))
		{
			//bitBufferLength最多64位
			uint64_t tmp = (byte << (b_it * 8));
			bitBuffer |= tmp;
		}
		else
		{
			//文件中数据不够了。
			validBitsNum = b_it * 8;
			return validBitsNum;
		}
	}

	validBitsNum = bitBufferLength;

	return validBitsNum;
}

//调用这个接口的前提，byteBuffer中的数据都被用完了。
uint64_t ReadBuffer::getBytesFromFile()
{
	filePtr->read((char*)byteBuffer, byteBufferLength);

	//判断出实际得到的byte的数量。
	validByteNum = filePtr->gcount();

	//初始化其他从参数
	bytePos = 0;

	//test
	cout << "getBytesFromFile: read number " << validByteNum << endl;
	//end test
	return validByteNum;
}




//==================bitfile=====================

BitReadFile::BitReadFile( bool state)
{
	path = "";
    this->state = state;
}

BitReadFile::~BitReadFile()
{
	//delete path;
}

bool BitReadFile::isOpen()
{
	return file.is_open();
}

bool BitReadFile::open(string filePath)
{
	file.open(filePath, ios::binary);
	return true;
}

void BitReadFile::close()
{
	file.close();
}

bool BitReadFile::getBit(bool & b_out)
{
	//状态是read的状态
	assert(!state);

	return true;
}

void BitReadFile::putBit(bool & b_in)
{
	//状态是write状态
	assert(state);

}

void BitReadFile::putBit(bool & b_in, const int & n)
{
	//状态是write状态
	assert(state);

}


