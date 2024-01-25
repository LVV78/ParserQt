#include  "Exceptions.h"
#include "ReaderBase.h"
#include <cstring>
void ReaderBase::close()
{
}

void ReaderBase::setEof(bool value)
{
	eof_ = value;
}

ReaderBase::ReaderBase(const char* blockSeparator, size_t bufferSize)
{
	bufferPos_ = 0;
	bufferSize_ = bufferSize;
	size_t length = strlen(blockSeparator);
	blockSeparator_ = new char[length + 1];
	if (length > 0)
	{
		strcpy(blockSeparator_, blockSeparator);
		blockCharSeparator_ = blockSeparator_[length - 1];
		blockSeparatorLength_ = length;
	}
	buffer_ = new char[bufferSize_];
	blockBuffer_ = new char[bufferSize_ + 1];
}

ReaderBase::~ReaderBase()
{
	delete[] buffer_;
	delete[] blockBuffer_;
	delete[] blockSeparator_;
}

size_t ReaderBase::bufferSize()
{
	return bufferSize_;
}

char* ReaderBase::blockBuffer()
{
	return blockBuffer_;
}

char* ReaderBase::buffer()
{
	return buffer_;
}



inline bool ReaderBase::isStop(char value)
{
	return stopChar_ == value;
}

inline  bool ReaderBase::isBlockSeparator(char value)
{
	if (blockCharSeparator_ == value)
	{
		auto ofs = blockSeparatorLength_ - 1;
		if (blockPos_ >= ofs)
		{
			auto start = blockPos_ - ofs;
			auto s = &blockBuffer_[start];
			auto res = memcmp(blockSeparator_, s, blockSeparatorLength_);
			if (res == 0)
			{
				return true;
			}
		}
	}
	return false;
}

char ReaderBase::getChar()
{
	if (bufferPos_ >= readCount_)
	{
		if (eof_)
		{
			return stopChar_;
		}
		readCount_ = readBuffer();
		if (readCount_ == 0)
		{
			setEof(true);
			return stopChar_;
		}
		bufferPos_ = 0;
	}
	return  buffer_[bufferPos_++];
}

void ReaderBase::doOpen()
{
	if (!opened_)
	{
		open();
		opened_ = true;
		size_ = 0;
		bufferPos_ = bufferSize_;
		blockCount_ = 0;
	}
}

void ReaderBase::doClose()
{
	if (opened_)
	{
		close();
	}
}

bool ReaderBase::read()
{
	doOpen();
	blockPos_ = 0;
	char c;

	while (true)
	{
		c = getChar();
		if (!isStop(c))
		{
			size_++;
			if (blockPos_ >= bufferSize_)
			{
				doClose();
				throw ProviderException(format(PE_BLOCK_TOO_LARGE, bufferSize_));
			}
			blockBuffer_[blockPos_] = c;
			if (isBlockSeparator(c))
			{
				blockCount_++;
				blockLength_ = blockPos_ - blockSeparatorLength_ + 1;
				return true;
			}
    		blockPos_++;
		}
		else
		{
			if (blockPos_ > 0)
			{
				blockCount_++;
				blockLength_ = blockPos_;
				return true;
			}
			else
			{
				doClose();
				return false;
			}
		}
	}
}

size_t ReaderBase::blockCount()
{
	return blockCount_;
}

size_t ReaderBase::blockLength()
{
	return blockLength_;
}
