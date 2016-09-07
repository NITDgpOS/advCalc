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
		E_noError    =  0,
		E_memAlloc   = -1,
		E_outOfRange = -2,
		E_invalidCmd = -3
	};
	const char *toString();
	bool isSet();
	void set(const signed char);
	void reset();
};

extern ERROR Error;

#endif
