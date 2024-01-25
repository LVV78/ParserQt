#include "StringReader.h"

void StringReader::open()
{
}

void StringReader::close()
{
}

size_t StringReader::readBuffer()
{
	size_t end = pos_ + bufferSize();
	size_t ts = text_.size();
	if (end > ts)
	{
		end = ts;
	}
	size_t len = end - pos_;
	std::memcpy(blockBuffer(), text_.c_str() + pos_, len);
	pos_ += len;
	return len;
}

StringReader::StringReader(const std::string& text, const char* blockSeparator, size_t bufferSize) :ReaderBase(blockSeparator, bufferSize)
{
	text_ = text;
}
