#include "Exceptions.h"

ProviderException::ProviderException(std::string message) : message_{ message }
{}

std::string ProviderException::getMessage() const
{
	return message_;
}

ProviderPositionException::ProviderPositionException(std::string message, int pos) : ProviderException{ message }, pos_{ pos }
{}

std::string ProviderPositionException::getMessage() const
{
	return ProviderException::getMessage() + "Position=" + std::to_string(pos_);
}

ParserException::ParserException(std::string message, char* buffer, int length) :ProviderException{ message }
{
	text_ = "\"" + std::string(buffer, length) + "\"";
}

std::string ParserException::getMessage() const
{
	return ProviderException::getMessage() + "\r\n" + text_;
}

bool stringReplace(std::string& str, const std::string& from, const std::string& to)
{
	size_t startPos = str.find(from);
	if (startPos == std::string::npos)
		return false;
	str.replace(startPos, from.length(), to);
	return true;
}

std::string format(std::string message, const std::string& text)
{
	stringReplace(message, "{}", text);
	return message;
}
std::string format(std::string message, size_t value)
{
	return format(message, std::to_string(value));
}

