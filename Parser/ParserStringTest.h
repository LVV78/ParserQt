#pragma once
#include "ParserTemplate.h"
#include <string>
#include <vector>

class ParserStringTest : public ParserTemplate<std::vector<std::string>>
{
protected:
	void parseBlock() override;

public:
	ParserStringTest(ReaderBase* reader) :ParserTemplate(reader) {}
	
	
};