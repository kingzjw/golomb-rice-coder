#include "GolombEncoder.h"

GolombEncoder::GolombEncoder(uint64_t k)
{
	this->k = k;
	//��Ϊ��Golomb Rice 
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

	// �ռ��㹻����ű�������Ԫ�򷵻�true�����򷵻�false
	return bitStream.freeLength() >= len; 
}

bool GolombEncoder::close()
{
	return bitStream.flush();
}


// Golomb Rice
uint64_t GolombEncoder::rice_golombEncode(uint64_t num)
{
	//���ƣ��൱�� q = num/ (x^k)
	uint64_t q = num >> k;
	//�൱�� r = num - q * (x^k)
	uint64_t r = num & (m - 1);

	// �������Ԫ�ĳ���
	auto len = q + 1 + k;

	//�����жϻ����Ƿ�Ϊ����ֱ��������ţ�����Ļ����浽bit buffer��
	bitStream.putBit(1, q);
	bitStream.putBit(0);

	for (int i = 0; i < k; i++)
	{
		//�ж�r�����һλ�������1����ôputBit true,�����0 ,��ôputBit false;
		bitStream.putBit(static_cast<bool>(r & 0x01));
		//r����һλ��
		r >>= 1;
	}

	return len; // ���ر������Ԫ�ĳ���
}

// Exp Golomb
uint64_t GolombEncoder::exp_golombEncode(uint64_t num)
{
	if (k != 0)
		num = num + pow(2, k) - 1;

	// 0��exp-golom����
	auto m = static_cast<int>(log(num + 1) / log(2)); // ǰ׺��Ҳ�������
	auto info = static_cast<int>(num + 1 - pow(2, m)); // ���ڵ�ƫ����

	auto len = 2 * m + 1; // �������Ԫ�ĳ���

						  // д��m��0
	bitStream.putBit(0, m);
	// д��1
	bitStream.putBit(1);

	// д��info m-1 bits
	for (int i = 0; i < m; i++)
	{
		bitStream.putBit(static_cast<bool>(info & 0x01));
		info >>= 1;
	}

	return len;
}
