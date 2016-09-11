#ifndef answerMANAGER
#define answerMANAGER

#include <calcStack.hpp>

template <typename Type>
class answerManager
{
	calcStack<Type> *answerStack;
	unsigned int numOfStacks;
	unsigned int ansPerStack;
	unsigned long numOfAns;
	bool autoDelete;
	bool increaseSize();
	bool shift();
public:
	~answerManager();
	answerManager();
	answerManager(const unsigned int);
	answerManager(const unsigned int, const unsigned int);
	answerManager(const bool, const unsigned long);
	answerManager(const bool, const unsigned int, const unsigned int);
	void toggleAutoDelete();
	bool getAns(Type &) const;
	bool push(const Type);
};

template <typename Type>
bool answerManager<Type>::increaseSize()
{
	if (++numOfStacks == 0) return Error = ERROR::outOfRange, --numOfStacks, 0;
	try
	{
		calcStack<Type> *tmp = new calcStack<Type>[numOfStacks];
		for (unsigned int i = 0; i < numOfStacks - 1; ++i)
			tmp[i] = answerStack[i];
		tmp[i].setCapacity(ansPerStack);
		delete [] answerStack;
		answerStack = tmp;
		return 1;
	}
	catch (bad_alloc x)
	{
		return Error = ERROR::memAlloc, 0;
	}
}

template <typename Type>
bool answerManager<Type>::shift()
{
	try
	{
		calcStack<Type> *tmp = new calcStack<Type>[numOfStacks];
		for (unsigned int i = 1; i < numOfStacks; ++i)
			tmp[i - 1] = answerStack[i];
		delete [] answerStack;
		answerStack = tmp;
		return numOfAns -= ansPerStack, 1;
	}
	catch (bad_alloc x)
	{
		return Error = ERROR::memAlloc, 0;
	}
}

template <typename Type>
answerManager<Type>::answerManager()
{
	delete [] answerStack;
}

template <typename Type>
answerManager<Type>::answerManager()
{
	numOfStacks = 1000;
	answerStack = new calcStack<Type>[numOfStacks];
	ansPerStack = 10000;
	for (unsigned int i = 0; i < numOfStacks; ++i)
		answerStack[i].setCapacity(ansPerStack);
	numOfAns = 0;
	autoDelete = false;
}

template <typename Type>
answerManager<Type>::answerManager(const unsigned int nos)
{
	numOfStacks = nos;
	answerStack = new calcStack<Type>[numOfStacks];
	ansPerStack = 10000;
	for (unsigned int i = 0; i < numOfStacks; ++i)
		answerStack[i].setCapacity(ansPerStack);
	numOfAns = 0;
	autoDelete = false;
}

template <typename Type>
answerManager<Type>::answerManager(const unsigned int nos, const unsigned int aps)
{
	numOfStacks = nos;
	answerStack = new calcStack<Type>[numOfStacks];
	ansPerStack = aps;
	for (unsigned int i = 0; i < numOfStacks; ++i)
		answerStack[i].setCapacity(ansPerStack);
	numOfAns = 0;
	autoDelete = false;
}

template <typename Type>
answerManager<Type>::answerManager(const bool ad, const unsigned long maxLt)
{
	autoDelete = ad;
	numOfStacks = maxLt / 10000 + (autoDelete ? 2 : 1);
	ansPerStack = 10000;
	numOfAns = 0;
	answerStack = new calcStack<Type>[numOfStacks];
	for (unsigned int i = 0; i < numOfStacks; ++i)
		answerStack[i].setCapacity(ansPerStack);
}

template <typename Type>
answerManager<Type>::answerManager(const bool ad, const unsigned int nos, const unsigned int aps)
{
	autoDelete = ad;
	numOfStacks = nos + (autoDelete ? 1 : 0);
	answerStack = new calcStack<Type>[numOfStacks];
	ansPerStack = aps;
	for (unsigned int i = 0; i < numOfStacks; ++i)
		answerStack[i].setCapacity(ansPerStack);
	numOfAns = 0;
}

template <typename Type>
bool answerManager<Type>::push(const Type x)
{
	if (this->answerStack[this->numOfAns++ / this->numOfStacks].push(x))
	{
		if (this->numOfAns == this->numOfStacks * this->ansPerStack)
			return autoDelete ? this->shift() : this->increaseSize();
		return 1;
	}
	return 0;
}

template <typename Type>
void answerManager<Type>::toggleAutoDelete()
{
	autoDelete = autoDelete ? 0 : 1;
}

template <typename Type>
bool answerManager<Type>::getAns(Type &x) const
{
}

#endif
