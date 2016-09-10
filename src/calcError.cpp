#include <calcError.hpp>

ERROR Error;

ERROR::ERROR()
{
	this->e = 0;
}

ERROR::ERROR(const signed char error)
{
	this->e = error;
}

const char *ERROR::toString()
{
	switch (this->e)
	{
	case noError   : return "No error";
	case memAlloc  : return "Unable to allocate memory";
	case outOfRange: return "Out of range";
	case numScarce : return "Number Scarcity error";
	case optrScarce: return "Operator Scarcity error";
	case brktError : return "Bracket Error";
	case invalidCmd: return "Invalid command";
	default        : return "Error not recognized";
	}
}

bool ERROR::isSet()
{
	return this->e;
}

void ERROR::set(const signed char error)
{
	this->e = error;
}

void ERROR::reset()
{
	this->e = noError;
}
