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
	case   0: return "No error";
	case  -1: return "Unable to allocate memory";
	case  -2: return "Out of range";
	case  -3: return "Invalid command";
	default : return "Error not recognized";
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
	this->e = E_noError;
}
