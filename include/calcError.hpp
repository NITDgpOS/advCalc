#ifndef calcERROR
#define calcERROR

class ERROR
{
private:
	signed char e;

public:
	ERROR();
	ERROR(const signed char);
	enum
	{
		noError    =  0,
		memAlloc   = -1,
		outOfRange = -2,
		invalidCmd = -3
	};
	const char *toString();
	bool isSet();
	void set(const signed char);
	void reset();
};

extern ERROR Error;

#endif
