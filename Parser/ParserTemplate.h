#pragma once
#include "ParserBase.h"
#ifndef PROVIDER_TEMPLATE_H
#define PROVIDER_TEMPLATE_H

template <typename T>
class ParserTemplate : public ParserBase
{
private:
	T* data_;
public:
    ParserTemplate(ReaderBase* reader) :ParserBase(reader)
	{
		data_ = new T();
	}
    ~ParserTemplate()
	{
		delete data_;
	}
	T* getData();

};



template<typename T>
T* ParserTemplate<T>::getData()
{
	return data_;

}
#endif
