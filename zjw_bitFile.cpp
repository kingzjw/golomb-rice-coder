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
		//bitbuffer�е����ݶ������꣬��Ҫ��bytebuffer�л�ȡ���ݡ�
		if (getNextBytesFromByteBuffer()>0)
		{
			bit = bitBuffer[bitPos];
			bitPos++;
			return true;
		}
		else
		{
			//bytebuffer��û���㹻��������
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
			//byte buffer�е����ݲ�������Ҫ��ȡ���ݡ�
			byte = byteBuffer[bytePos];
			bytePos++;
			return true;
		}
		else
		{
			//û���㹻��������
			bytePos = -1;
			return false;
		}
	}
}

//ֻ��֤bitBufferLength������64��ʱ���ǶԵ�,��������ӿ�֮ǰ��֤��bitbuffer�е������Ѿ����ꡣ 
uint8_t ReadBuffer::getNextBytesFromByteBuffer()
{
	assert(bitPos >= validBitsNum);

	//init bitBuffer ��Ϊ0
	bitBuffer.reset();
	
	//��byteBuffer�ж�ȡ���ɸ���Ч���ݡ�
	for (int b_it = 0; b_it < bitBufferLength / 8; b_it++)
	{
		uint8_t  byte;
		if (getByte(byte))
		{
			//bitBufferLength���64λ  ����ŵ���0���ұ�
			uint64_t tmp = (byte << (b_it * 8));
			bitBuffer |= tmp;
		}
		else
		{
			//�ļ������ݲ����ˡ�
			validBitsNum = b_it * 8;
			bitPos = 0;
			return validBitsNum;
		}
	}

	bitPos = 0;
	validBitsNum = bitBufferLength;

	return validBitsNum;
}

//��������ӿڵ�ǰ�ᣬbyteBuffer�е����ݶ��������ˡ�
uint64_t ReadBuffer::getBytesFromFile()
{
	filePtr->read((char*)byteBuffer, byteBufferLength);

	//�жϳ�ʵ�ʵõ���byte��������
	validByteNum = filePtr->gcount();

	//��ʼ�������Ӳ���
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
		//bitBuffer д�뵽 bytebuffer
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

		//����8λ
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
		//bitbuffer��û������
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
		//byte buffer��Ҫд�� �ļ���,����ʼ��bytePos �� validByteNum
		putBytesBufferToFile();

		//�������Byte
		byte = byteBuffer[bytePos];
		bytePos++;
		validByteNum++;
	}
}

void WriteBuffer::putBitsToByteBuffer()
{
	uint8_t  byte;
	//�������8�����Ͳ�Ҫ���ˡ�
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
	//-------bit buffer����byte buffer------
	//��8λ��bits���byteд��ByteBuffer��
	putBitsToByteBuffer();
	
	//�������8λ��Ҳ��0���byte buffer��
	if (validBitsNum > 0)
	{
		uint8_t  byte;
		getOneByteFromBitBuffer(byte);
		putOneByteToByteBuffer(byte);
	}
	
	//-------bytebuffer  ���� file-------
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
	//״̬��read��״̬
	assert(!state);

	return true;
}

void BitReadFile::putBit(bool & b_in)
{
	//״̬��write״̬
	assert(state);

}

void BitReadFile::putBit(bool & b_in, const int & n)
{
	//״̬��write״̬
	assert(state);

}
