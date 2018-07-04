/*
* author: Jiawei Zhou
* time: 2018/7/2
* head file: 只需要给一个二进制的文件名，就可以不断的通过接口按位读取内容。也可以按位保存内容。
*			 对外隐藏缓冲等功能。/Users/king/Desktop/compress/zjw_bitFile.cpp
* using example:
*	BitReadFile class: 
*		1. open must be called
*		2. getBit() call by user 
*		3. close must be called
*	BitWriteFile
*		1. open must be called
*		2. putBit() 
*		3. close must be called
*	
*/

#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <assert.h>

using namespace std;

//bitBufferLength 一定要是8的倍数
static const int bitBufferLength = 8; 
static const int byteBufferLength = 512; 

//BitBuffer 是一个字节的容量。
class ReadBuffer
{
private:
	ifstream *filePtr;

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
    ReadBuffer(ifstream *filePtr);
	~ReadBuffer();

    //返回false表示，没有拿到bit.功能：从bit buffer 中取出的一个bit，当bitbuffer不够的时候，自动从byte buffer中取新的数据
    bool getBit(bool &bit);

private:
	//从bytebuffer中只拿一个byte，返回值如果是false说明，文件中数据不够了。
	bool getByteFromByteBuffer(uint8_t & byte);

    //返回的是保存到bit buffer的数据数量， 从byte buffer拿一个新的byte放到bit buffer中
    uint8_t getNextBytesFromByteBuffer();
	
	//当byteBuffer中都用过的时候，需要重新从文件读取内容。返回的是得到的有效byte的数量
	uint64_t getBytesFromFile();

};

//BitBuffer 是一个字节的容量。
class WriteBuffer
{
private:
	ofstream *filePtr;

	// 使用bitset缓存bit
	std::bitset<bitBufferLength> bitBuffer;
	// 当前bit的有效位置
	int  bitPos;
	// 已经存有的bit 的个数
	uint64_t validBitsNum;

	//unsigned char Byte数据
	uint8_t *byteBuffer;
	// 当前byte的有效位置
	uint64_t  bytePos;
	// 总的放入到 byte buffer中的字节数.到末尾的时候，只有少部分。
	uint64_t validByteNum;

private:

	//从bit buffer中获取一个byte，获取之后已经移除这个byte了。
	bool getOneByteFromBitBuffer(uint8_t & byte);

    // 把8个bits打成byte放到Byte中。不满8个bits仍然留在bitbuffer中
    void putBitsToByteBuffer();

	void putOneByteToByteBuffer(uint8_t byte);

    // 向bitset中写入一个byte
    void putBytesBufferToFile();

public:
	// 构造函数
	WriteBuffer(ofstream *filePtr);

	~WriteBuffer();

	// 向bitset中写入一个bit
	void putBit(bool b);
	
	// 向bitset中写入n个这样的bits
	void putBit(bool b, int n);

	//结束之前必须调用这个函数
	void fillout();
};


/*
* 对外的接口类.提供按位的操作。
*/
class BitReadFile
{
private:
	//要读取，或者要写入的二进制文件
	ifstream file;
	string path;
	ReadBuffer * readBuffer;
    
public:

	BitReadFile(string path);
	~BitReadFile();

	bool isOpen();
	bool open(string filePath);
	bool open();
	void close();

	//返回值是：是否得到一个bit,如果false 表示到了文件末尾。参数是返回值
	bool getBit(bool & b_out);
};

/*
* 对外的接口类.提供按位的操作。
*/
class BitWriteFile
{
private:
	//要读取，或者要写入的二进制文件
	ofstream file;
	string path;
	WriteBuffer * writeBuffer;
public:
	BitWriteFile(string path);
	~BitWriteFile();

	bool isOpen();
	bool open(string filePath);
	bool open();
	void close();

	//写人一个Bit
	void putBit(bool b_in);
	void putBit(int b_in);


	//同一个元素，同时存放多次
	void putBit(bool b_in, int n);
	void putBit(int  b_in, int n);


};