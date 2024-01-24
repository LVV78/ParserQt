#include  "Exceptions.h"
#include  "ParserBase.h"
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

void ReaderBase::setProvider(ParserBase* provider)
{
	provider_ = provider;
}

size_t ReaderBase::bufferSize()
{
	return bufferSize_;
}

char* ReaderBase::blockBuffer()
{
	return blockBuffer_;
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
		bufferPos_ = 0;
	}
	return  buffer_[bufferPos_++];
}
void ReaderBase::endBlock()
{
	blockPos_ -= blockSeparatorLength_;
	provider_->parseBlockBegin(blockPos_ + 1, blockCount_);
	blockPos_ = 0;
	blockCount_++;
}
void ReaderBase::read()
{
	open();
	size_ = 0;
	bufferPos_ = bufferSize_;
	blockCount_ = 0;
	blockPos_ = 0;
	char c = 0;
	while (true)
	{
		c = getChar();
		if (!isStop(c))
		{
			size_++;
			if (blockPos_ > bufferSize_)
			{
				close();
				throw ProviderException(format(PE_BLOCK_TOO_LARGE, bufferSize_));
			}
			blockBuffer_[blockPos_] = c;
			if (isBlockSeparator(c))
			{
				endBlock();
			}
			else
			{
				blockPos_++;
			}
		}
		else
		{
			if (blockPos_ > 0)
			{
				blockCount_++;
				endBlock();
			}
			break;
		}
	}
	close();
}
size_t ReaderBase::blockCount()
{
	return blockCount_;
}


