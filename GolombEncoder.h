#pragma once

#include "BitStream.h"

class GolombEncoder
{
private:

	BitOutputStream bitStream;
	uint64_t m;
	uint64_t k;
public:

	//����Kֵ
	GolombEncoder(uint64_t k);

	//���룬����ǰ���ȼ���������Ԫ�ĳ���L����������泤�Ȳ���L bits����byte������
	//��ʣ���bit��BitBuffer�л��棬Ȼ�󷵻�false��ָ�����治���Դ�ű�������Ԫ��
	bool encode(uint64_t num);

	//����������̡���bit buffer�е�����flush��byte buffer�У�Ȼ�����64bits��0��
	//���ж�totalCodeLength�Ƿ���8�ı�����������ǣ��������0.
	bool close();
	
	//buffer�����Ժ󣬴�������buffer���ݣ�д���ļ�������...��������buffer������ʹ�á�
	void resetBuffer();
	//���ⲿ�趨buffer
	void setBuffer(uint8_t *buffer, uint64_t len);

	// �����Ѿ�������ֽ�byte���ȣ�����Ϊ8�ı���
	uint64_t getToalCodeLength()
	{
		return bitStream.getTotalCodeLength();
	}

private:
	//����Golomb Rice �� num���б��룬����ֵ�Ǳ�������Ԫ����
	uint64_t rice_golombEncode(uint64_t num);
	
	//Exp Golomb
	uint64_t exp_golombEncode(uint64_t num);

};
