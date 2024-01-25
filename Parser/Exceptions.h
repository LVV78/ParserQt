#pragma once
#include <string>
#ifndef PROVIDER_EXCEPTION_H
#define PROVIDER_EXCEPTION_H

class ProviderException : public std::exception
{
public:
	ProviderException(std::string message);
	virtual std::string getMessage() const;

private:
	std::string message_;
};

class ProviderPositionException : public ProviderException
{
public:
	ProviderPositionException(std::string message, size_t pos);
	std::string getMessage() const override;
private:
	size_t pos_;
};

class ParserException : public ProviderException
{
public:
	ParserException(std::string message, char* buffer, size_t length);
	std::string getMessage() const override;
private:
	std::string text_;
};

bool stringReplace(std::string& str, const std::string& from, const std::string& to);

std::string format(std::string message, const std::string& text);
std::string format(std::string message, size_t value);

const std::string PE_FILE_NOT_FOUND = "File not found. \"{}\".";
const std::string PE_NAME_NOT_FOUND = "Variable \"{}\" not found.";
const std::string PE_VALUE_NOT_FOUND = "Value not found.";
const std::string PE_EMPTY_ARRAY = "Array is empty.";
const std::string PE_BLOCK_TOO_LARGE = "Reading block too large. Buffer szie : \"{}\".";

#endif
