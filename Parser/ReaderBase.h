//Basic reading module. It allows to read abstracted data from any source
#pragma once
#include <stddef.h>

#ifndef SAMPLE_BASE_H
#define SAMPLE_BASE_H

class ReaderBase
{
private:
	char stopChar_ = 0;
	bool opened_ = false;
	size_t size_ = 0;
	char* blockBuffer_ = nullptr;
	char* buffer_ = nullptr;
	char* blockSeparator_ = nullptr;
	char blockCharSeparator_ = stopChar_;
	size_t blockSeparatorLength_ = 0;
	size_t blockPos_ = 0;
	size_t blockLength_ = 0;
	size_t blockCount_ = 0;
	size_t bufferSize_;
	bool eof_ = false;

	inline bool isStop(char value);
	inline bool isBlockSeparator(char value);
	inline char getChar();
	void doOpen();
	void doClose();
protected:
	size_t bufferPos_;
	size_t readCount_ = 0;

	virtual void open() = 0;
	virtual void close();
	virtual size_t readBuffer() = 0;
	void setEof(bool value);
public:
	ReaderBase(const char* blockSeparator, size_t bufferSize = 1000);
	virtual ~ReaderBase();

	size_t bufferSize();
	size_t blockCount();
	size_t blockLength();
	char* blockBuffer();
	char* buffer();
	bool read();
};

#endif
