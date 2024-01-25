//Reader implementation of file reader
#pragma once
#include <fstream>
#include "ReaderBase.h"

class FileReader : public ReaderBase
{
private:
	const char* fileName_;
	std::ifstream file_;
protected:
	void open() override;
	void close() override;
	size_t readBuffer() override;
public:
	FileReader(const char* fileName, const char* blockSeparator, size_t bufferSize = 1000);
};
