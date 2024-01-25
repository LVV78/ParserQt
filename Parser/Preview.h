#pragma once
#include <string>
#include <vector>

enum PhraseType
{
	ptNone,
	ptName,
	ptValue,
	ptArray,
	ptError
};

class PreviewItem
{
public:
	size_t start;
	size_t length;
	PhraseType phraseType = ptNone;
};

class Preview
{
public:
	std::vector<PreviewItem>* items_ = new std::vector<PreviewItem>();
	std::string Text;
	~Preview();
	void add(size_t start, size_t length, PhraseType phraseType);
	void clear();

};