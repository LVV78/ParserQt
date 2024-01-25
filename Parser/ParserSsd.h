#pragma once
#include "ParserTemplate.h"
#include <string>
#include <vector>

class SsdData
{
public:
	std::vector<Point>* values_ = new std::vector<Point>();
	std::string frequncy, date, single, filter, common, refOsc, statistics, dly, measuringTime;
	std::vector<std::string> source, inputA, inputB, extArm, holdOff;
	PointMinMax minMax;

	void Add(Point value)
	{
		values_->push_back(value);
	}
	~SsdData()
	{
		delete values_;
	}
};

class ParserSsd : public ParserTemplate<SsdData>
{
private:
	char separator_ = 0;
	int headerNo_ = 0;
	void parseHeader(int headerNo);
	void parseData();
	char getSeparator();
protected:
	void parseBlock() override;
public:
	ParserSsd(ReaderBase* reader) :ParserTemplate(reader) {}

	size_t count() override
	{
		return getData()->values_->size();
	}
	Point points(size_t index) override
	{
		auto v = getData()->values_->at(index);
		return Point{ v.x, v.y };

	}
	PointMinMax minMax() override
	{
		return getData()->minMax;
	}
};

