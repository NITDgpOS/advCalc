#include <ctype.h>

#include <calcOptr.hpp>

static const char bin_ops[22][4] =
{
	"+", "-", "*", "/", "^", "|", "&", "P", "C", ">>", "<<",
	"<", ">", ">=", "<=", "==", "!=", "%", "log", "&&", "||", ""
};

static const char un_ops[22][7] =
{
	"~", "!", "ln", "abs", "cos", "sin", "tan", "acos", "asin", "atan",
	"sec", "cosec", "cot", "asec", "acosec", "acot", "logten", "floor",
	"sinh", "cosh", "tanh", "ceil"
};

static optr_hash bin_ops_hash[22] = { 0 };
static optr_hash un_ops_hash[22] = { 0 };

static const unsigned long unify = 999999999;

Operator::Operator()
{
	this->op = this->priority = 0;
	this->isBinary = false;
}

Operator::Operator(optr_hash x)
{
	this->op = x;
	this->setOperatorProperties();
}

Operator::Operator(const char *x)
{
	if (not this->setFromString(x))
	{
		this->op = this->priority = 0;
		this->isBinary = 0;
	}
}

Operator::Operator(const Operator &x)
{
	this->op = x.op;
#ifdef TESTING
	this->setOperatorProperties();
	if (x.isBinary != this->isBinary ||
		x.priority != this->priority)
		throw "Operator error caught\n"
			"Most recent call to Operator::setOperatorProperties";
#else
	this->isBinary = x.isBinary;
	this->priority = x.priority;
#endif
}

inline bool Operator::isUnary() const
{
#ifdef TESTING
	if (not this->op)
		throw "Error: Null operator beign checked for unary";
#endif
	return not this->isBinary;
}

Operator Operator::operator=(const Operator &x)
{
	this->op = x.op;
#ifdef TESTING
	this->setOperatorProperties();
	if (x.isBinary != this->isBinary ||
		x.priority != this->priority)
		throw "Operator error caught\n"
			"Most recent call to Operator::setOperatorProperties";
#else
	this->isBinary = x.isBinary;
	this->priority = x.priority;
#endif
	return *this;
}

static str_hash generateHashKey(
	const char *s, unsigned long start = 0, unsigned long end = 0)
{
	register const char *x = s = s + start;
	register str_hash hash = 0;
	if (start == end)
		while (*x)
			hash = 127 * hash + *(x++);
	else if (start < end)
		while (start < end)
			hash = 127 * hash + *(x++), ++start;
	else
		return 0;
	return hash;
}

bool ismathchar(const char ch)
{
	if (isalpha(ch) ||
		ch == '/' ||
		ch == '*' ||
		ch == '-' ||
		ch == '+' ||
		ch == '^' ||
		ch == '%' ||
		ch == '_' ||
		ch == '!' ||
		ch == '&' ||
		ch == '|' ||
		ch == '<' ||
		ch == '>' ||
		ch == '~' ||
		ch == '=')
		return 1;
	return 0;
}

static int priorityOf(const optr_hash s)
{
	/*
	  priority list:  
	  const char operators[][5][4] = {
	  { "&&", "||" },
	  { ">", "<", ">=", "<=", "==", "!=" },
	  { "+", "-" },
	  { "*", "/" },
	  { "%", "^" },
	  { "P", "C", "log" },
	  { "&", "|" },
	  { ">>", "<<" }
	  };
	*/
	switch (s)
	{
	case Operator::H_and:
	case Operator::H_or:
		return 1;

	case Operator::H_great:
	case Operator::H_less:
	case Operator::H_equal:
	case Operator::H_not_equal:
	case Operator::H_great_equal:
	case Operator::H_less_equal:
		return 2;

	case Operator::H_plus:
	case Operator::H_minus:
		return 3;

	case Operator::H_multiply:
	case Operator::H_divide:
		return 4;

	case Operator::H_mod:
	case Operator::H_pow:
		return 5;

	case Operator::H_P:
	case Operator::H_C:
	case Operator::H_log:
		return 6;

	case Operator::H_bit_and:
	case Operator::H_bit_or:
		return 7;

	case Operator::H_bit_shift_right:
	case Operator::H_bit_shift_left:
		return 8;

	default:
		return 0;
	}
}

bool Operator::setOperatorProperties()
{
	if (not op) return 0;
	register unsigned char beg = 0, end = 21, mid;
	bool found = 0;
	while (beg <= end)
	{
		mid = (beg + end) / 2;
		if (this->op == un_ops_hash[mid] ||
			this->op == un_ops_hash[beg] ||
			this->op == un_ops_hash[end])
		{
			found = 1;
			break;
		}
		else if (this->op < un_ops_hash[mid])
			end = mid - 1, ++beg;
		else
			beg = mid + 1, --end;
	}
	if (found)
		isBinary = false;
	else
	{
		beg = 0, end = 20, mid;
		while (beg <= end)
		{
			mid = (beg + end) / 2;
			if (this->op == bin_ops_hash[mid] ||
				this->op == bin_ops_hash[beg] ||
				this->op == bin_ops_hash[end])
			{
				found = 1;
				break;
			}
			else if (this->op < bin_ops_hash[mid])
				end = mid - 1, ++beg;
			else
				beg = mid + 1, --end;
		}
		if (not found) return 0;
		isBinary = true;
		this->priority = priorityOf(op);
	}
	return 1;
}

unsigned char Operator::setFromString(const char *start)
{
	int charsRead = 0;
	str_hash hash = 0;
	Operator optr;
	for (int i = 0; ismathchar(*start) && i < 6; ++i)
	{
		hash = 127 * hash + *(start++);
		optr.op = hash % unify;
		charsRead = optr.setOperatorProperties() ? *this = optr, i : charsRead;
	}
	return charsRead;
}

const char *Operator::toString() const
{
	switch (this->op)
	{
	case H_plus:              return "+";
	case H_minus:             return "-";
	case H_multiply:          return "*";
	case H_divide:            return "/";
	case H_pow:               return "^";
	case H_mod:               return "%";
	case H_P:                 return "P";
	case H_C:                 return "C";
	case H_bit_and:           return "&";
	case H_bit_or:            return "|";
	case H_bit_shift_right:   return ">>";
	case H_bit_shift_left:    return "<<";
	case H_bit_not:           return "~";
	case H_open_bracket:      return "(";
	case H_close_bracket:     return ")";
	case H_not:               return "!";
	case H_and:               return "&&";
	case H_or:                return "||";
	case H_great:             return ">";
	case H_less:              return "<";
	case H_great_equal:       return ">=";
	case H_less_equal:        return "<=";
	case H_sin:               return "sin";
	case H_cos:               return "cos";
	case H_tan:               return "tan";
	case H_sec:               return "sec";
	case H_cosec:             return "cosec";
	case H_cot:               return "cot";
	case H_asin:              return "asin";
	case H_acos:              return "acos";
	case H_atan:              return "atan";
	case H_asec:              return "asec";
	case H_acosec:            return "acosec";
	case H_acot:              return "acot";
	case H_sinh:              return "sinh";
	case H_cosh:              return "cosh";
	case H_tanh:              return "tanh";
	case H_log:               return "log";
	case H_ln:                return "ln";
	case H_logten:            return "logten";
	case H_abs:               return "abs";
	case H_floor:             return "floor";
	case H_ceil:              return "ceil";
	case 0:                   return "(null)";
	default:                  return "";
	}
}

void makeOperatorHashes()
{
	int i = 0, j;
	optr_hash t;

	/* Generating */
	for (; i < 22; ++i)
	{
		bin_ops_hash[i] = generateHashKey(bin_ops[i]) % unify;
		un_ops_hash[i] = generateHashKey(un_ops[i]) % unify;
	}

	/* Sorting */
	for (i = 0; i < 22; ++i)
		for (j = 0; j < 22 - i - 1; ++j)
			if (un_ops_hash[j] > un_ops_hash[j + 1])
			{
				t = un_ops_hash[j];
				un_ops_hash[j] = un_ops_hash[j + 1];
				un_ops_hash[j + 1] = t;
			}
	for (i = 0; i < 21; ++i)
		for (j = 0; j < 21 - i - 1; ++j)
			if (bin_ops_hash[j] > bin_ops_hash[j + 1])
			{
				t = bin_ops_hash[j];
				bin_ops_hash[j] = bin_ops_hash[j + 1];
				bin_ops_hash[j + 1] = t;
			}
}

#define HIGH 20
#define LOW  10
unsigned char Operator::checkPriority(const Operator s2) const
{
#ifdef TESTING
	if (not this->op || not s2.op)
		throw "Error: Null operator cannot have a priority";
#endif
	bool s1_open = this->op == H_open_bracket;
	bool s2_close = s2.op == H_close_bracket;
	if (s1_open && s2_close)
		return HIGH;

	if (s2_close)
		return LOW;

	if (s1_open)
		return HIGH;

	if (this->isBinary && not s2.isBinary)
		return HIGH;

	if (not this->isBinary && s2.isBinary)
		return LOW;

	if (not this->isBinary && not s2.isBinary)
		return HIGH;

	if (this->op == s2.op)
	{
		if (this->isBinary)
			return LOW;
		return HIGH;
	}

	if (this->priority && s2.priority)
		return this->priority < s2.priority ? HIGH : LOW;
}

inline bool Operator::operator==(const optrHash x) const
{
	return this->op == x;
}

inline bool Operator::operator==(const Operator x) const
{
	return this->op == x.op;
}

inline bool Operator::operator!=(const optrHash x) const
{
	return this->op != x;
}

inline bool Operator::operator!=(const Operator x) const
{
	return this->op != x.op;
}

inline bool Operator::operator>(const Operator x) const
{
	return this->checkPriority(x) == HIGH;
}

inline bool Operator::operator<(const Operator x) const
{
	return this->checkPriority(x) == LOW;
}
