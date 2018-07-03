#include "zjw_bitFile.h"

//=====================ReadBuffer=======================
ReadBuffer::ReadBuffer(fstream * filePtr)
{
	assert(filePtr);
	assert(bitBufferLength % 8 == 0);


	//file
	this->filePtr = filePtr;

	//bit 
    bitBuffer.reset();
	bitPos = -1;
	validBitsNum = -1;

	//byte
	byteBuffer = new uint8_t[byteBufferLength];
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
			bitPos++;
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
		bytePos++;
		return true;
	}
	else
	{
		if (getBytesFromFile()>0)
		{
			//byte buffer中的数据不够，需要读取数据。
			byte = byteBuffer[bytePos];
			bytePos++;
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
	
	//从byteBuffer中读取若干个有效数据。
	for (int b_it = 0; b_it < bitBufferLength / 8; b_it++)
	{
		uint8_t  byte;
		if (getByte(byte))
		{
			//bitBufferLength最多64位  逆序放到。0在右边
			uint64_t tmp = (byte << (b_it * 8));
			bitBuffer |= tmp;
		}
		else
		{
			//文件中数据不够了。
			validBitsNum = b_it * 8;
			bitPos = 0;
			return validBitsNum;
		}
	}

	bitPos = 0;
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

//=====================writeBuffer=======================



WriteBuffer::WriteBuffer(fstream * filePtr)
{
	assert(filePtr); 
	assert(bitBufferLength % 8 == 0);

	//file
	this->filePtr = filePtr;

	//bit 
	bitBuffer.reset();
	bitPos = 0;
	validBitsNum = 0;

	//byte
	byteBuffer = new uint8_t[byteBufferLength];
	bytePos = -1;
	validByteNum = 0;

}

WriteBuffer::~WriteBuffer()
{
	delete filePtr;
}


void WriteBuffer::putBit(bool b)
{
	if (bitPos < bitBufferLength)
	{
		bitBuffer[bitPos] = b;
		bitPos++;
		validBitsNum++;
	}
	else
	{
		//bitBuffer 写入到 bytebuffer
		putBitsToByteBuffer();
	}

}

void WriteBuffer::putBit(bool b, int n)
{
	for (int i = 0; i < n; i++)
	{
		putBit(b);
	}
}

bool WriteBuffer::getOneByteFromBitBuffer(uint8_t & byte)
{
	if (validBitsNum >= 8)
	{
		byte = static_cast<uint8_t>(bitBuffer.to_ulong());

		//右移8位
		bitBuffer >>= 8;
		bitPos -= 8;
		validBitsNum -= 8;
		return true;

	}
	else if (validBitsNum > 0)
	{
		byte = static_cast<uint8_t>(bitBuffer.to_ulong());
		bitBuffer >>= 8;
		bitPos = 0;
		validBitsNum = 0;

		return true;
	}
	else
	{
		//bitbuffer中没有数据
		return false;
	}

}

void WriteBuffer::putOneByteToByteBuffer(uint8_t & byte)
{
	if (bytePos < byteBufferLength)
	{
		byte = byteBuffer[bytePos];
		bytePos++;
		validByteNum++;
	}
	else
	{
		//byte buffer需要写到 文件中,并初始化bytePos 和 validByteNum
		putBytesBufferToFile();

		//保存这个Byte
		byte = byteBuffer[bytePos];
		bytePos++;
		validByteNum++;
	}
}

void WriteBuffer::putBitsToByteBuffer()
{
	uint8_t  byte;
	//如果少于8个，就不要放了。
	while (validBitsNum >= 8)
	{
		getOneByteFromBitBuffer(byte);
		putOneByteToByteBuffer(byte);
	}
}

void WriteBuffer::putBytesBufferToFile()
{
	filePtr->write((char*)byteBuffer, validByteNum);
	bytePos = 0;
	validByteNum = 0;
}

void WriteBuffer::fillout()
{
	//-------bit buffer都到byte buffer------
	//把8位的bits打成byte写到ByteBuffer中
	putBitsToByteBuffer();
	
	//如果不满8位，也补0打进byte buffer中
	if (validBitsNum > 0)
	{
		uint8_t  byte;
		getOneByteFromBitBuffer(byte);
		putOneByteToByteBuffer(byte);
	}
	
	//-------bytebuffer  都到 file-------
	putBytesBufferToFile();	
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
