#include "ParserBase.h"
#include "ParserSsd.h"
#include "Exceptions.h"

void ParserSsd::parseBlock()
{

	skipSpaces();

	switch (getChar())
	{
	case '#':
		incPos();
		parseHeader(headerNo_++);
		break;
	default:
		parseData();
	}
}

void ParserSsd::parseHeader(int headerNo)
{
	std::string phrase, name;
	std::vector<std::string> arr;
	Separators rightSeparator = { {' ',' '},{'\t'} };
	Separators nameSeparator{ {':'} };
	Separators columnSeparator{ {' '}, {'\t'} };
	Separators arraySeparator{ {','} };
	switch (headerNo)
	{
	case 0:
		getData()->source = readValues(arraySeparator, rightSeparator, false, true);
		break;
	case 1:

	{
		readName("FREQUENCY", columnSeparator, true);
		getData()->frequncy = readValue(columnSeparator, true);

	}
	break;
	case 2:

	{
		getData()->date = readValue({}, true);
		break;
	}
	case 3:
	{
		readName("Measuring time", nameSeparator, true);
		getData()->measuringTime = readValue(rightSeparator, true);
		readName("Single", nameSeparator, true);
		getData()->single = readValue(columnSeparator, true);
	}
	break;
	case 4:
	{
		readName("Input A", nameSeparator, true);
		getData()->inputA = readValues(arraySeparator, rightSeparator, true, false);
		readName("Filter", nameSeparator, true);
		getData()->filter = readValue(columnSeparator, true);
	}
	break;
	case 5:
	{
		readName("Input B", nameSeparator, true);
		getData()->inputB = readValues(arraySeparator, rightSeparator, true, false);
		readName("Common", nameSeparator, true);
		getData()->common = readValue(columnSeparator, true);
	}
	break;
	case 6:
		readName("Ext.arm", nameSeparator, true);
		getData()->extArm = readValues(arraySeparator, rightSeparator, true, false);
		readPhrase(phrase, nameSeparator, true);
		if (compareName(phrase, "Dly"))
		{
			getData()->dly = tryReadValue(rightSeparator, true);
			readPhrase(phrase, nameSeparator, true);
		}

		if (compareName(phrase, "Ref.osc"))
		{
			getData()->refOsc = readValue(columnSeparator, true);
		}
		else
		{
			parserError(PE_NAME_NOT_FOUND, "Ref.osc");
		}

		break;
	case 7:
	{
		readName("Hold off", nameSeparator, true);
		getData()->holdOff = readValues(arraySeparator, rightSeparator, true, false);
		readName("Statistics", nameSeparator, true);
		getData()->statistics = readValue(columnSeparator, true);
	}
	break;
	default:
		break;
	}
}

void ParserSsd::parseData()
{
	if (eob())
	{
		return;
	}
	char sep = getSeparator();
	int len;
	char* buf;
	len = readPhrase(buf, sep, true);
	if (len != 0)
	{
		auto d = getData();
		Point ssd;
		ssd.x = toDouble(buf, len);
		d->minMax.setX(ssd.x);
		len = readPhrase(buf, sep, true);
		if (len != 0)
		{
			ssd.y = toDouble(buf, len);
			d->minMax.setY(ssd.y);
			d->Add(ssd);
			return;

		}
	}
	parserError(PE_VALUE_NOT_FOUND);
}

char ParserSsd::getSeparator()
{
	if (separator_ != 0)
	{
		return separator_;
	}

	do
	{
		char c = getChar();
		switch (c)
		{
		case ' ':
		case '\t':
			separator_ = c;
			setPos(0);
			return c;
		default:
			break;
		}

	} while (incPos());
	parserError("Separator not found.");
	return 0;
}
