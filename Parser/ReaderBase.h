#pragma once
#include <stddef.h>

#ifndef SAMPLE_BASE_H
#define SAMPLE_BASE_H

class ParserBase;

class ReaderBase
{
private:
	char stopChar_ = 0;

	size_t size_ = 0;
	char* blockBuffer_;
	char* blockSeparator_;
	char blockCharSeparator_ = stopChar_;
	size_t blockSeparatorLength_;
	size_t blockPos_, blockCount_;
	size_t bufferSize_;
	bool eof_ = false;
    ParserBase* provider_;

	inline bool isStop(char value);
	inline bool isBlockSeparator(char value);
	inline char getChar();
	void endBlock();
protected:
	size_t bufferPos_;
	char* buffer_;
	size_t readCount_ = 0;
	virtual void open() = 0;
	virtual void close();
	virtual size_t readBuffer() = 0;
	void setEof(bool value);
public:
	ReaderBase(const char* blockSeparator, size_t bufferSize = 1000);
	virtual ~ReaderBase();
    void setProvider(ParserBase* provider);
	size_t bufferSize();
	size_t blockCount();
	char* blockBuffer();
	void read();
};

#endif
