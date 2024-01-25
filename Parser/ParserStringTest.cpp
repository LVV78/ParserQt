#include "ParserStringTest.h"

void ParserStringTest::parseBlock()
{
	if (eob())
	{
		return;
	}
	readName("Name", { {' '} }, true);
	std::string val = readValue({ {} }, true);
	getData()->push_back(val);
}
