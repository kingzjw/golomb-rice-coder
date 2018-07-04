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
	//判断是否run length的k
	k = 0;

	//default, L一定是2的pow
	L = 4;
	kRP = 16;

	//kR = kRP/L;
	kR = kRP >> ((int)log2(L));
	//Golomb Rice 
	m = pow(2, kR);

	//四个参数
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

	//对每一个数字进行编码
	for (int num_it = 0; num_it < codeData.size(); num_it++)
	{
		u = codeData[num_it];
		if (k = 0)
		{
			//no run model  GR的方式code.
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
					//数字的连续为m个0000000，编码为0
					//把多取的那个数字退回去
					num_it--;
					//encode 为0
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
					//数字的连续为uIsZero个0000000 加上一个u，编码为0
					encodeStringWithZeros(uIsZero, u);
					updateKR();
					updateK();
				}
				else
				{
					//u= 0。这种情况是可能出现在最后几个数字上的，0的个数不够的情况。
					assert(0);
					//需要处理这种情况
					//add something
				}
			}
			else if (u > 0)
			{
				// run model 下面遇到单个非0的数字，前面没有0的
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

	//关闭
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
		//p不可能小于0
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
			//不可能小于0
			assert(0);
		}
	}
	else if(k > 0)
	{
		//根据run的类型进行操作

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
		//不可能小于0
		assert(0);
	}
}

bool RLGR::rice_golombEncode(uint64_t num)
{
	//右移，相当于 q = num/ (x^k)
	p = num >> kR;
	//相当于 r = num - q * (x^k)
	uint64_t r = num & (m - 1);

	// 编码后码元的长度
	auto len = p + 1 + kR;

	//不会判断缓存是否为满，直接向里面放，不足的话缓存到bit buffer中
	bitWriteFile->putBit(1, (int)p);
	bitWriteFile->putBit(0);

	//余数是反着来存的。
	for (int i = 0; i < kR; i++)
	{
		//判断r的最低一位，如果是1，那么putBit true,如果是0 ,那么putBit false;
		bitWriteFile->putBit(static_cast<bool>(r & 0x01));
		//r右移一位。
		r >>= 1;
	}
	return true;
}

bool RLGR::encodeOneZero()
{
	bitWriteFile->putBit(0);
	return true;
}

//zeroNum 表示0的个数
bool RLGR::encodeStringWithZeros(int zeroNum ,uint64_t &u)
{
	bitWriteFile->putBit(1);
	//zeroNum 用k为来表示成二进制.低位先存的
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

	//余数是先存低位的，所以解析出来也是从低位开始解析
	for (int i = 0; i < k; i++)
	{
		bitReadFile->getBit(b);
		bits.set(i, b);
	}

	num = unary * m + bits.to_ulong();

	return true;
}
