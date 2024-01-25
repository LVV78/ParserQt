#include "StringReader.h"
#include <cstring>
void StringReader::open()
{
}

void StringReader::close()
{
}

size_t StringReader::readBuffer()
{
	size_t end = pos_ + bufferSize() - 1;
	size_t ts = text_.size();
	if (end >= ts)
	{
		end = ts - 1;
		setEof(true);
	}
	size_t len = end - pos_ + 1;
	std::memcpy(buffer(), text_.c_str() + pos_, len);
	pos_ = end + 1;
	return len;
}

StringReader::StringReader(const std::string& text, const char* blockSeparator, size_t bufferSize) :ReaderBase(blockSeparator, bufferSize)
{
	text_ = text;
}
