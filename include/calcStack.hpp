#ifndef calcSTACK
#define calcSTACK

#include <algorithm>
#include <calcError.hpp>

template <typename Type>
class stack
{
	Type *start;
	Type *current;
	long unsigned size;
	long unsigned rate;
	bool accelerate;
public:
	stack();
	stack(const long unsigned size);
	stack(const bool);
	stack(const long unsigned, const long unsigned, const bool);
	stack(const stack &);
	~stack();
	void beFast(bool);
	void operator=(const stack &);
	bool increaseSize();
	bool decreaseSize();
	bool get(Type &);
	bool pop();
	bool pop(Type &);
    bool push(const Type);
	bool push(const Type *, const Type *);
	void reset();
};

template <typename Type>
stack<Type>::stack()
{
	this->size       = 16;
	this->rate       = 1;
	this->accelerate = 1;
	this->start      = new Type[this->size];
	this->current    = 0;
}

template <typename Type>
stack<Type>::stack(const long unsigned size)
{
	this->size       = size;
	this->rate       = 1;
	this->accelerate = 1;
	this->start      = new Type[this->size];
	this->current    = 0;
}

template <typename Type>
stack<Type>::stack(const bool accelerate)
{
	this->size       = 16;
	this->rate       = 1;
	this->accelerate = accelerate;
	this->start      = new Type[this->size];
	this->current    = 0;
}

template <typename Type>
stack<Type>::stack(const unsigned long size, const long unsigned rate, const bool accelerate)
{
	this->size       = size;
	this->rate       = rate;
	this->accelerate = accelerate;
	this->start      = new Type[this->size];
	this->current    = 0;
}

template <typename Type>
stack<Type>::stack(const stack<Type> &t)
{
	this->size       = t.size;
	this->rate       = t.rate;
	this->accelerate = t.accelerate;
	this->start      = new Type[this->size];
	this->current    = t.current ? this->start + (t.current - t.start) : 0;
	std::copy(t.start, t.current, this->start);
}

template <typename Type>
void stack<Type>::operator=(const stack<Type> &t)
{
	if (this->start)
		delete [] this->start;
	this->start = new Type[this->size = t.size];
	this->rate = t.rate;
	this->accelerate = t.accelerate;
	this->current = t.current ? this->start + (t.current - t.start) : 0;
	std::copy(t.start, t.current, this->start);
}

template <typename Type>
stack<Type>::~stack()
{
	this->reset();
	delete [] this->start;
}

template <typename Type>
void stack<Type>::beFast(bool acc)
{
	this->accelerate = acc;
}

template <typename Type>
bool stack<Type>::pop(Type &y)
{
	if (current)
	{
		y = *(--current);
		if (start == current)
			current = 0;
		return 1;
	}
	return 0;
}

template <typename Type>
bool stack<Type>::pop()
{
	if (this->current)
	{
		if (this->start == --this->current)
			this->current = 0;
		return 1;
	}
	return 0;
}

template <typename Type>
bool stack<Type>::get(Type &y)
{
	return current ? y = *(current - 1), 1 : 0;
}

template <typename Type>
bool stack<Type>::push(const Type y)
{
	if (current)
	{
		if ((long unsigned)(current - start) == size)
			if (not this->increaseSize())
				return 0;
		*(current++) = y;
	}
	else
		*((current = start + 1) - 1) = y;
	return 1;
}

template <typename Type>
bool stack<Type>::push(const Type *start, const Type *end)
{
	while (this->size < end - start)
		if (not this->increaseSize())
			return 0;
	if (this->current)
	{
		std::copy(start, end, this->current - 1);
		this->current += end - start;
	}
	else
	{
		std::copy(start, end, this->start);
		this->current = this->start + (end - start);
	}
	return 1;
}

template <typename Type>
void stack<Type>::reset()
{
	current = 0;
}

template <typename Type>
bool stack<Type>::decreaseSize()
{
	try
	{
		Type *temp;
		if (accelerate)
		{
			if (rate - 1 > rate) return Error = ERROR::outOfRange, 0;
			temp = new Type[size/accelerate];
		}
		else
		{
			if (size - rate > size)	return Error = ERROR::outOfRange, 0;
			temp = new Type[size - rate];
		}
		size = accelerate ? size / --rate : size - rate;
		if (current)
		{
			std::copy(start, start + size, temp);
			current = temp + size;
		}
		delete [] this->start;
		start = temp;
		return 1;
	}
	catch(std::bad_alloc x)
	{
		return Error = ERROR::memAlloc, 0;
	}
}

template <typename Type>
bool stack<Type>::increaseSize()
{
	try
	{
		Type *temp = accelerate ? new Type[(rate + 1) * size] : new Type[rate + size];
		size = accelerate ? size + (++rate) : size + rate;
		if (current)
		{
			std::copy(start, current, temp);
			current = temp + (current - start);
		}
		delete [] this->start;
		start = temp;
		return 1;
	}
	catch(std::bad_alloc x)
	{
		return Error = ERROR::memAlloc, 0;
	}
}

#endif
