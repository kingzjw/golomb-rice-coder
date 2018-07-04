#include "zjw_RLGR.h"


RLGR::RLGR(vector<uint64_t> &codeData, vector<uint64_t> &resData, RunType type,std::string fileName)
{
	initParam();
	
	this->type = type;
	this->fileName = fileName;
	bitReadFile = new BitReadFile(fileName);
	bitWriteFile = new BitWriteFile(fileName);
}

RLGR::~RLGR()
{
	if (bitReadFile)
		delete bitReadFile;
	if (bitWriteFile)
		delete bitWriteFile;
}

void RLGR::initParam()
{
	//�ж��Ƿ�run length��k
	k = 0;

	//default, Lһ����2��pow
	L = 4;
	kRP = 16;

	//kR = kRP/L;
	kR = kRP >> ((int)log2(L));
	//Golomb Rice 
	m = pow(2, kR);

	//�ĸ�����
	u0 = 3;
	d0 = 1;
	u1 = 2;
	d1 = 1;
	kP = 16;
}

void RLGR::encode()
{
	//open
	bitWriteFile->open();

	//��ÿһ�����ֽ��б���
	for (int num_it = 0; num_it < codeData.size(); num_it++)
	{
		u = codeData[num_it];
		if (k = 0)
		{
			//no run model  GR�ķ�ʽcode.
			rice_golombEncode(u);
			updateKR();
			updateK();
		}
		else if (k > 0)
		{
			//run model 
			if (u == 0)
			{
				u = codeData[++num_it];
				int uIsZero = 1;
				while (u == 0 && uIsZero < m)
				{
					uIsZero++;
					u = codeData[++num_it];
				}

				if (uIsZero == m)
				{
					//���ֵ�����Ϊm��0000000������Ϊ0
					//�Ѷ�ȡ���Ǹ������˻�ȥ
					num_it--;
					//encode Ϊ0
					encodeOneZero();

					//update
					updateKR();
					updateK();

					continue;
				}
				else if(u != 0)
				{
					//u != 0
					assert(u);
					//���ֵ�����ΪuIsZero��0000000 ����һ��u������Ϊ0
					encodeStringWithZeros(uIsZero, u);
					updateKR();
					updateK();
				}
				else
				{
					//u= 0����������ǿ��ܳ�������󼸸������ϵģ�0�ĸ��������������
					assert(0);
					//��Ҫ�����������
					//add something
				}
			}
			else if (u > 0)
			{
				// run model ��������������0�����֣�ǰ��û��0��
				rice_golombEncode(u);
				updateKR();
				updateK();
			}
		}
		else
		{
			assert(0);
		}
	}

	//�ر�
	bitWriteFile->close();
}

bool RLGR::decode(uint64_t & num)
{
	return rice_golombDecode(num);
}

void RLGR::setK(uint64_t k_)
{
	k = k_;
	m = pow(2, k);
}

void RLGR::updateKR()
{
	if (p == 0)
	{
		kRP = kRP - 2;
		kR = kRP >> ((int)log2(L));
		m = pow(2, kR);
	}
	else if (p == 1)
	{
		//no change
	}
	else if(p > 1)
	{
		kRP = kRP +p+1;
		kR = kRP >> ((int)log2(L));
		m = pow(2, kR);
	}
	else
	{
		//p������С��0
		assert(0);
	}
}

void RLGR::updateK()
{
	if (k == 0)
	{
		if (u == 0)
		{
			kP = kP + u0;
			k = kP >> ((int)log2(L));
		}
		else if (u > 0)
		{
			kP = kP - d0;
			k = kP >> ((int)log2(L));
		}
		else
		{
			//������С��0
			assert(0);
		}
	}
	else if(k > 0)
	{
		//����run�����ͽ��в���

		switch (type)
		{
		case CompleteRun:
			kP = kP + u1;
			k = kP >> ((int)log2(L));
			break;
		case PartialRun:
			kP = kP - d1;
			k = kP >> ((int)log2(L));
			break;
		default:
			assert(0);
		}
	}
	else
	{
		//������С��0
		assert(0);
	}
}

bool RLGR::rice_golombEncode(uint64_t num)
{
	//���ƣ��൱�� q = num/ (x^k)
	p = num >> kR;
	//�൱�� r = num - q * (x^k)
	uint64_t r = num & (m - 1);

	// �������Ԫ�ĳ���
	auto len = p + 1 + kR;

	//�����жϻ����Ƿ�Ϊ����ֱ��������ţ�����Ļ����浽bit buffer��
	bitWriteFile->putBit(1, (int)p);
	bitWriteFile->putBit(0);

	//�����Ƿ�������ġ�
	for (int i = 0; i < kR; i++)
	{
		//�ж�r�����һλ�������1����ôputBit true,�����0 ,��ôputBit false;
		bitWriteFile->putBit(static_cast<bool>(r & 0x01));
		//r����һλ��
		r >>= 1;
	}
	return true;
}

bool RLGR::encodeOneZero()
{
	bitWriteFile->putBit(0);
	return true;
}

//zeroNum ��ʾ0�ĸ���
bool RLGR::encodeStringWithZeros(int zeroNum ,uint64_t &u)
{
	bitWriteFile->putBit(1);
	//zeroNum ��kΪ����ʾ�ɶ�����.��λ�ȴ��
	for (int i = 0; i < k; i++)
	{
		bitWriteFile->putBit(zeroNum & 0x01);
		zeroNum >> 1;
	}

	rice_golombEncode(u - 1);

	return true;
}

bool RLGR::rice_golombDecode(uint64_t & num)
{
	bool b;
	uint64_t unary = 0;
	bitReadFile->getBit(b);
	while (b)
	{
		unary++;
		bitReadFile->getBit(b);
	}

	std::bitset<64> bits;
	bits.reset();

	//�������ȴ��λ�ģ����Խ�������Ҳ�Ǵӵ�λ��ʼ����
	for (int i = 0; i < k; i++)
	{
		bitReadFile->getBit(b);
		bits.set(i, b);
	}

	num = unary * m + bits.to_ulong();

	return true;
}