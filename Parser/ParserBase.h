#pragma once
#include <string>
#include <cctype>
#include <vector>
#include  "Preview.h"
#include  <float.h>

#ifndef PROVIDER_BASE_H
#define PROVIDER_BASE_H

struct Point
{
	double x, y;
};
struct PointMinMax
{
	double minX = DBL_MAX, minY = DBL_MAX, maxX = DBL_MIN, maxY = DBL_MIN;
	void reset()
	{
		minX = DBL_MAX, minY = DBL_MAX, maxX = DBL_MIN, maxY = DBL_MIN;
	}
	void setX(double value)
	{
		if (value < minX)
		{
			minX = value;
		}
		if (value > maxX)
		{
			maxX = value;
		}
	}
	void setY(double value)
	{
		if (value < minY)
		{
			minY = value;
		}
		if (value > maxY)
		{
			maxY = value;
		}
	}
	double width()
	{
		return maxX - minX;
	}
	double height()
	{
		return maxY - minY;
	}
};
typedef  std::vector<std::vector<char>> Separators;
typedef std::vector<std::string> StringArray;
typedef void(*previewPtr)(Preview* previrew, std::string&);

inline bool icharEquals(char a, char b)
{
	return std::tolower(static_cast<unsigned char>(a)) ==
		std::tolower(static_cast<unsigned char>(b));
}
std::string trim(const std::string& str);
class ReaderBase;
class ParserBase
{
private:
	size_t pos_ = 0, length_ = 0, phraseStart_ = 0, blockNo_ = 0;
	ReaderBase* reader_ = nullptr;
	char* buffer_;
	bool eob_ = false, bob_ = false;
	Preview* preview_ = nullptr;
	size_t lastLength_;

	char* buffer();
protected:
	inline bool incPos();
	size_t blockNo();
	size_t pos();
	size_t getLengthAtPos();
	bool setPos(size_t value);
	void virtual skipSpaces();
	inline char getChar() const;
	bool eob();
	size_t readPhrase(char*& phrase, char separator, bool skipSpaces);

	std::vector<char> readPhrase(std::string& phrase, Separators separators, bool skipSpaces);
	std::vector<char> readArray(StringArray& array, Separators arraySeparators, Separators separators, bool trimValues, bool skipSpaces);

	bool readName(std::string name, Separators separators, bool skipSpaces);
	std::string readValue(Separators separators, bool skipSpaces);
	std::string tryReadValue(Separators separators, bool skipSpaces);
	StringArray readValues(Separators arraySeparators, Separators separators, bool trimValues, bool skipSpaces);
	StringArray tryReadValues(Separators arraySeparators, Separators separators, bool trimValues, bool skipSpaces);
	bool compareName(const std::string& a, const std::string& b, bool caseSensitive = false);

	void parserError(std::string message, std::string value);
	void parserError(std::string message);
	char* toChar(char* value, size_t length);

	virtual void parseBlock()
	{

	}
public:
    ParserBase(ReaderBase* reader);
    virtual ~ParserBase();

	previewPtr previewCallback = nullptr;

	bool in(std::vector<char> value, Separators separators);
	void parseBlockBegin(size_t length, size_t blockNo);

	double toDouble(char* value, size_t length);
	std::string toString(char* value, size_t length);

	virtual size_t count()
	{
		return 0;
	}
	virtual Point points(size_t index)
	{
		return  Point();
	}
	virtual PointMinMax minMax()
	{
		return PointMinMax();
	}
};



#endif
inline char ParserBase::getChar() const
{
	if (eob_)
		return 0;
	return buffer_[pos_];
}
inline bool ParserBase::incPos()
{
	if (pos_ >= length_ - 1)
	{
		eob_ = true;
		return false;
	}
	pos_++;
	return true;
}
