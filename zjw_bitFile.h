﻿/*
* author: Jiawei Zhou
* time: 2018/7/2
* head file: 只需要给一个二进制的文件名，就可以不断的通过接口按位读取内容。也可以按位保存内容。
*			 对外隐藏缓冲等功能。/Users/king/Desktop/compress/zjw_bitFile.cpp
*/
#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include<fstream>
#include <assert.h>

using namespace std;

//bitBufferLength 一定要是8的倍数
static const int bitBufferLength = 8; 
static const int byteBufferLength = 128; 

//BitBuffer 是一个字节的容量。
class ReadBuffer
{
public:
	fstream *filePtr;

	//==================bit========================

	// 使用bitset缓存bit
    std::bitset<bitBufferLength> bitBuffer;
	// 当前bit的有效位置
    int  bitPos;  
	// 有效的bitBuffer 中的bit数.到末尾的时候，只有少部分。
	uint64_t validBitsNum;

	//==================byte========================

	//unsigned char Byte数据
	uint8_t *byteBuffer;
    // 当前byte的有效位置
	uint64_t  bytePos;
	// 总的放入到 byte buffer中的字节数.到末尾的时候，只有少部分。
	uint64_t validByteNum; 
    
public:
    // 构造函数
    ReadBuffer(fstream *filePtr);
	~ReadBuffer();


    //返回false表示，没有拿到bit.功能：从bit buffer 中取出的一个bit，当bitbuffer不够的时候，自动从byte buffer中取新的数据
    bool getBit(bool &bit);

	//从bytebuffer中只拿一个byte，返回值如果是false说明，数据不够了
	bool getByte(uint8_t & byte);

    //返回的是保存到bit buffer的数据数量， 从byte buffer拿一个新的byte放到bit buffer中
    uint8_t getNextBytesFromByteBuffer();
	
	//当byteBuffer中都用过的时候，需要重新从文件读取内容。返回的是得到的有效byte的数量
	uint64_t getBytesFromFile();

};

//BitBuffer 是一个字节的容量。
class WriteBuffer
{
public:
	fstream *filePtr;

	//==================bit========================

	// 使用bitset缓存bit
	std::bitset<bitBufferLength> bitBuffer;
	// 当前bit的有效位置
	int  bitPos;

	//==================byte========================

	//unsigned char Byte数据
	uint8_t *byteBuffer;
	// 当前byte的有效位置
	uint64_t  bytePos;
	// 数据长度
	uint64_t length;
	// 总的放入到 byte buffer中的字节数
	uint64_t totalLength;

public:
	// 构造函数
	WriteBuffer(fstream *filePtr);
	~WriteBuffer();

	//=========================bits=================

	
	// 向bitset中写入一个bit
	void putBit(bool b);
	// 向bitset中写入一个byte
	void putByte(uint8_t b);

	//=========================byte===============
    // 向bitset中写入一个bit
    void putBit(bool b);
    // 向bitset中写入一个byte
    void putByte(uint8_t b);

};

/*
* 对外的接口类.提供按位的操作。
* 切换读写，要改变state，而且要重新打开关闭文件。
*/
class BitReadFile
{
private:
	//要读取，或者要写入的二进制文件
	fstream file;
	string path;
    //true: write, false: read
    bool state;

public:

	BitReadFile( bool state);
	~BitReadFile();

	bool isOpen();
	bool open(string filePath);
	void close();

	//返回值是：是否成功。参数是返回值
	bool getBit(bool & b_out);

	//写人一个Bit
	void putBit(bool & b_in);

	//同一个元素，同时存放多次
	void putBit(bool & b_in, const int& n);

};

class BitWriteFile
{
private:
	//要读取，或者要写入的二进制文件
	fstream file;
	string path;
	//true: write, false: read
	bool state;

public:

	BitReadFile(bool state);
	~BitReadFile();

	bool isOpen();
	bool open(string filePath);
	void close();

	//返回值是：是否成功。参数是返回值
	bool getBit(bool & b_out);

	//写人一个Bit
	void putBit(bool & b_in);

	//同一个元素，同时存放多次
	void putBit(bool & b_in, const int& n);

};