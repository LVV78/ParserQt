#pragma once
#pragma once
#include <string>
#include "ReaderBase.h"

class StringReader : public ReaderBase
{
private:
	std::string text_;
	size_t pos_ = 0;
protected:
	void open() override;
	void close() override;
	size_t readBuffer() override;
public:
	StringReader(const std::string& text, const char* blockSeparator, size_t bufferSize = 1000);
};