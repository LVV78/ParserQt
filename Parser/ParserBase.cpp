#include "ReaderBase.h"
#include "ParserBase.h"
#include "Exceptions.h"
#include <cstring>

void ParserBase::skipSpaces()
{
	if (eob_)
	{
		return;
	}
	do
	{
		switch (getChar())
		{
		case '\t':
		case ' ':
			break;
		default:
			return;
		}

	} while (incPos());
}

size_t ParserBase::readPhrase(char*& phrase, char separator, bool skipSpaces)
{
	if (eob_)
	{
		return 0;
	}
	if (skipSpaces)
	{
		ParserBase::skipSpaces();
	}
	lastLength_ = 0;
	phraseStart_ = pos_;
	do
	{
		auto c = getChar();
		if (c == separator)
		{
			phrase = &buffer_[phraseStart_];
			lastLength_ = pos_ - phraseStart_;
			incPos();
			return lastLength_;
		}

	} while (incPos());
	lastLength_ = pos_ - phraseStart_ + 1;
	phrase = &buffer_[phraseStart_];
	return lastLength_;
}

std::vector<char> ParserBase::readPhrase(std::string& phrase, Separators separators, bool skipSpaces)
{
	if (eob_)
	{
		phrase = "";
		return {};
	}
	if (skipSpaces)
	{
		ParserBase::skipSpaces();
	}
	lastLength_ = 0;
	phraseStart_ = pos_;

	do
	{

		for (auto item : separators)
		{
			int size = (int)item.size();
			int itemLeft = pos_ - size + 1;
			auto d = item.data();
			if (size > 0 && itemLeft >= phraseStart_ && itemLeft + size < length_)
			{
				auto res = memcmp(d, &buffer_[itemLeft], size);
				if (res == 0)
				{
					phrase = &buffer_[phraseStart_];
					lastLength_ = pos_ - phraseStart_ - (size - 1);
					phrase = std::string(&buffer_[phraseStart_], lastLength_);
					setPos(itemLeft + size);
					return item;
				}
			}
		}

	} while (incPos());
	lastLength_ = pos_ - phraseStart_ + 1;
	phrase = std::string(&buffer_[phraseStart_], lastLength_);
	return {};
}

std::vector<char> ParserBase::readArray(StringArray& array, Separators arraySeparators, Separators separators, bool trimValues, bool skipSpaces)
{
	array.clear();
	Separators sep;
	for (auto item : separators)
	{
		sep.push_back(item);
	}
	for (auto item : arraySeparators)
	{
		sep.push_back(item);
	}
	int arrStart = pos_;
	std::string phrase;
	while (true)
	{
		auto res = readPhrase(phrase, sep, skipSpaces);

		if (trimValues)
		{
			phrase = trim(phrase);
		}
		array.push_back(phrase);
		if (!in(res, arraySeparators) || eob_)
		{
			if (eob())
			{
				lastLength_ = pos_ - arrStart + 1;
			}
			else
			{
				lastLength_ = pos_ - arrStart;
			}
			return res;
		}
		else
		{
			incPos();
		}
	}
}

bool ParserBase::readName(std::string name, Separators separators, bool skipSpaces)
{
	std::string phrase;
	readPhrase(phrase, separators, skipSpaces);
	if (compareName(phrase, name))
	{
		preview_->add(phraseStart_, (int)lastLength_, ptName);
		return true;
	}
	parserError(PE_NAME_NOT_FOUND, name);

}

std::string ParserBase::readValue(Separators separators, bool skipSpaces)
{
	auto res = tryReadValue(separators, skipSpaces);
	if (res.empty())
	{
		parserError(PE_VALUE_NOT_FOUND);
	}
	return res;
}

std::string ParserBase::tryReadValue(Separators separators, bool skipSpaces)
{
	std::string res;
	readPhrase(res, separators, skipSpaces);
	preview_->add(phraseStart_, lastLength_, ptValue);
	return res;
}

StringArray ParserBase::readValues(Separators arraySeparators, Separators separators, bool trimValues, bool skipSpaces)
{
	auto res = tryReadValues(arraySeparators, separators, trimValues, skipSpaces);
	if (res.empty())
	{
		parserError(PE_EMPTY_ARRAY);
	}
	return res;
}

StringArray ParserBase::tryReadValues(Separators arraySeparators, Separators separators, bool trimValues, bool skipSpaces)
{
	std::vector<std::string> res;
	if (skipSpaces)
	{
		ParserBase::skipSpaces();
	}
	auto start = pos_;
	readArray(res, arraySeparators, separators, trimValues, skipSpaces);
	preview_->add(start, lastLength_, ptArray);
	return res;
}

char* ParserBase::buffer()
{
	return buffer_;
}

void ParserBase::callPreview()
{
	if (previewCallback != nullptr && preview_->items_->size() > 0)
	{
		std::string txt(buffer_, length_);
		previewCallback(preview_, txt);
	}
}

void ParserBase::parserError(std::string message, std::string value)
{
	preview_->add(phraseStart_, lastLength_, ptError);
	callPreview();
	auto posText = format(" At Block:{}", std::to_string(blockNo_)) + format(", Posdition:{}.", std::to_string(pos()));
	size_t len = pos_;
	if (eob())
	{
		len = pos_ + 1;
	}
	throw ParserException(format(message, value) + posText, buffer(), len);
}

void ParserBase::parserError(std::string message)
{
	parserError(message, "");
}

bool ParserBase::compareName(const std::string& a, const std::string& b, bool caseSensitive)
{
	bool res;
	if (caseSensitive)
	{
		res = a == b;
	}
	else
	{
		res = a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), icharEquals);
	}
	if (res)
	{
		preview_->add(phraseStart_, lastLength_, ptName);
	}
	return res;
}

bool ParserBase::in(std::vector<char> value, Separators separators)
{
	for (auto item : separators)
	{
		if (item == value)
		{
			return true;
		}
	}
	return false;
}

bool ParserBase::eob()
{
	return eob_;
}

ParserBase::ParserBase(ReaderBase* reader)
{
	reader_ = reader;
	buffer_ = reader->blockBuffer();
	preview_ = new Preview();
	lastLength_ = 0;
}

ParserBase::~ParserBase()
{
	delete reader_;
	delete preview_;
}

bool ParserBase::setPos(size_t value)
{
	if (value < 0)
	{
		bob_ = true;
		pos_ = 0;
		return false;
	}

	if (value >= length_)
	{
		eob_ = true;
		pos_ = length_ - 1;
		return false;
	}
	pos_ = value;
	return true;
}

size_t ParserBase::blockNo()
{
	return blockNo_;
}

size_t ParserBase::pos()
{
	return pos_;
}

void ParserBase::doParse(size_t length, size_t blockNo)
{
	pos_ = 0;
	eob_ = length == 0;
	bob_ = length == 0;
	length_ = length;
	blockNo_ = blockNo;
	preview_->clear();
	parseBlock();

	callPreview();
}

double ParserBase::toDouble(char* value, size_t length)
{
	char* buf = toChar(value, length);
	double res = atof(buf);
	delete[] buf;
	return res;
}

std::string ParserBase::toString(char* value, size_t length)
{
	auto res = std::string(value, length);
	return res;
}

void ParserBase::parse()
{
	while (reader_->read())
	{
		auto len = reader_->blockLength();
		auto blockNo = reader_->blockCount() - 1;
		doParse(len, blockNo);
	}
}

char* ParserBase::toChar(char* value, size_t length)
{
	char* res = new char[length + 1];
	res[length] = 0;
	std::memcpy(res, value, length);
	return res;
}

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}
