#include "Preview.h"

Preview::~Preview()
{
	delete items_;
}

void Preview::add(size_t start, size_t length, PhraseType phraseType)
{
	if (length == 0)
	{
		return;
	}
	PreviewItem res;
	res.start = start;
	res.length = length;
	res.phraseType = phraseType;
	items_->push_back(res);
}

void Preview::clear()
{
	items_->clear();
}

