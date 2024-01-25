#include "FileReader.h"
#include "Exceptions.h"

FileReader::FileReader(const char* fileName, const char* blockSeparator, size_t bufferSize) :ReaderBase(blockSeparator, bufferSize), fileName_(fileName)
{
}

void FileReader::open()
{
	file_.open(fileName_, std::ios::binary);
	if (!file_.is_open())
	{
		throw ProviderException(format(PE_FILE_NOT_FOUND, fileName_));
	}
}

void FileReader::close()
{
	if (file_.is_open())
	{
		file_.close();
	}
}

size_t FileReader::readBuffer()
{
	file_.read(buffer(), (std::streamsize)bufferSize());
	setEof(file_.eof());
	return (size_t)file_.gcount();
}
