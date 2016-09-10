#ifndef calcOPTR
#define calcOPTR

#include <calcStack.hpp>
#include <math.h>

typedef unsigned long long str_hash;
typedef unsigned int optr_hash;

class Operator
{
public:
	enum optrHash
	{
		H_plus            = 43,
		H_minus           = 45,
		H_multiply        = 42,
		H_divide          = 47,
		H_pow             = 94,
		H_mod             = 37,
		H_P               = 80,
		H_C               = 67,
		H_bit_and         = 38,
		H_bit_or          = 124,
		H_bit_shift_right = 7936,
		H_bit_shift_left  = 7680,
		H_bit_not         = 126,
		H_open_bracket    = 40,
		H_close_bracket   = 41,
		H_not             = 33,
		H_and             = 4864,
		H_or              = 15872,
		H_great           = 62,
		H_less            = 60,
		H_great_equal     = 7935,
		H_less_equal      = 7681,
		H_equal           = 7808,
		H_not_equal       = 4252,
		H_sin             = 1868280,
		H_cos             = 1610983,
		H_tan             = 1883393,
		H_sec             = 1867761,
		H_cosec           = 983557758,
		H_cot             = 1610984,
		H_asin            = 200561431,
		H_acos            = 200304134,
		H_atan            = 200576544,
		H_asec            = 200560912,
		H_acosec          = 705393442,
		H_acot            = 200304135,
		H_sinh            = 237271664,
		H_cosh            = 204594945,
		H_tanh            = 239191015,
		H_log             = 1756132,
		H_ln              = 13826,
		H_logten          = 232822054,
		H_abs             = 1577074,
		H_floor           = 757783302,
		H_ceil            = 204432389
	};
private:
	optr_hash op;
	bool isBinary;
	unsigned char priority;
	bool setOperatorProperties();
public:
	Operator();
	Operator(optr_hash);
	Operator(const char *);
	Operator(const Operator &);
	bool isUnary() const;
	const char *toString() const;
	unsigned char checkPriority(const Operator) const;
	unsigned char setFromString(const char *);
	Operator operator=(const Operator &);
	bool operator==(const Operator::optrHash) const;
	bool operator==(const Operator) const;
	bool operator!=(const Operator::optrHash) const;
	bool operator!=(const Operator) const;
	bool operator<(const Operator) const;
	bool operator>(const Operator) const;
};

#define RAD  0
#define DEG  1
#define GRAD 2

#define PI 3.14

extern unsigned char angle_type;

template <typename numType>
class operatorManager
{
	calcStack<Operator> operatorStack;
	calcStack<numType> numberStack;
	bool calculate(const Operator &, const numType, const numType);
public:
	bool insert(const Operator);
	void insert(const numType);
	bool finishCalculation();
	bool ans(numType &);
};

template <typename numType>
bool operatorManager<numType>::insert(const Operator z)
{
	Operator tmp;
	numType x, y;
	if (not this->operatorStack.get(tmp) || tmp > z)
		return this->operatorStack.push(z);
	do
	{
		if (not this->numberStack.pop(x))
			return Error = ERROR::numScarce, 0;
		if (not tmp.isUnary() && not this->numberStack.pop(y))
			return Error = ERROR::numScarce, 0;
	} while (this->calculate(tmp, x, y) && tmp < z);
	if (z != Operator::H_close_bracket)
		return this->operatorStack.push(z);
	return 1;
}

template <typename numType>
void operatorManager<numType>::insert(const numType x)
{
	this->numberStack.push(x);
}

template <typename numType>
bool operatorManager<numType>::finishCalculation()
{
	Operator tmp;
	numType x, y;
	if (this->operatorStack.get(tmp))
		do
		{
			if (not this->numberStack.pop(x))
				return Error = ERROR::numScarce, 0;
			if (not tmp.isUnary() && not this->numberStack.pop(y))
				return Error = ERROR::numScarce, 0;
		} while (this->calculate(tmp, x, y));
}

template <typename numType>
bool operatorManager<numType>::ans(numType &x)
{
#ifdef TESTING
	if (not this->operatorStack.isEmpty())
		throw "Operators left in stack due to some error";
#endif
	return this->numberStack.pop(x) && this->numberStack.isEmpty();
}

template <typename numType>
bool operatorManager<numType>::calculate(const Operator &op, const numType x, const numType y)
{
	numType z = angle_type == DEG ? (x * PI / 180) : (angle_type == RAD ? x : (x * PI / 200)), ans;

	/* Basic arithmatic operators */
	if (op == Operator::H_plus)
		ans = x + y;
	else if (op == Operator::H_minus)
		ans = x - y;
	else if (op == Operator::H_multiply)
		ans = x * y;
	else if (op == Operator::H_divide)
	{
		if (y)
			ans = x / y;
		else
			return Error = ERROR::divError, 0;
	}
	else if (op == Operator::H_pow)
		ans = powl(x, y);

	/* Factorials */
	else if (op == Operator::H_P)
	{
		if (x >= 0 && y >= 0 && x >= y && !(x - floorl(x)) && !(y - floorl(y)))
			ans = factorial(x) / factorial(x - y);
		else
			return Error = ERROR::factError, 0;
	}
	else if (op == Operator::H_C)
	{
		if (x >= 0 && y >= 0 && x >= y && !(x - floorl(x)) && !(y - floorl(y)))
			ans = factorial(x) / (factorial(y) * factorial(x - y));
		else
			return Error = ERROR::factError, 0;
	}

	/* Computer related basic operators */
	else if (op == Operator::H_bit_not)
		ans = ~(unsigned long)x;
	else if (op == Operator::H_bit_or)
		ans = (unsigned long)x | (unsigned long)y;
	else if (op == Operator::H_bit_and)
		ans = (unsigned long)x & (unsigned long)y;
	else if (op == Operator::H_mod)
		ans = fmodl(x, y);
	else if (op == Operator::H_bit_shift_right)
		ans = (unsigned long)x >> (unsigned long)y;
	else if (op == Operator::H_bit_shift_left)
		ans = (unsigned long)x << (unsigned long)y;

	/* Relational operators */
	else if (op == Operator::H_great)  ans = x > y;
	else if (op == Operator::H_less)  ans = x < y;
	else if (op == Operator::H_great_equal) ans = x >= y;
	else if (op == Operator::H_less_equal)  ans = x <= y;
	else if (op == Operator::H_not_equal)   ans = x != y;
	else if (op == Operator::H_equal)       ans = x == y;

	/* Other mathematical functions */
	else if (op == Operator::H_log)
	{
		if (y > 0 && x >= 0)
			ans = logl(y) / logl(x);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_abs)
		ans = fabsl(x);
	else if (op == Operator::H_ceil)
		ans = ceill(x);
	else if (op == Operator::H_floor)
		ans = floorl(x);
	else if (op == Operator::H_ln)
	{
		if (x > 0)
			ans = logl(x);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_logten)
	{
		if (x > 0)
			ans = log10l(x);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_sinh)
		ans = sinhl(z);
	else if (op == Operator::H_cosh)
		ans = coshl(z);
	else if (op == Operator::H_tanh)
		ans = tanhl(z);
	else if (op == Operator::H_sin)
		ans = sinl(z);
	else if (op == Operator::H_cos)
		ans = cosl(z);
	else if (op == Operator::H_tan)
	{
		if (cosl(z))
			ans = tanl(z);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_cosec)
	{
		if (sinl(z))
			ans = 1 / sinl(z);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_sec)
	{
		if (cosl(z))
			ans = 1 / cosl(z);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_cot)
	{
		if (sinl(z))
			ans = 1 / tanl(z);
		else
			return Error = ERROR::rangUndef, 0;
	}
	else if (op == Operator::H_asin)
	{
		if (x <= 1 && x >= -1)
			ans = angle_type == DEG ? (asinl(x) * 180 / PI) :
				angle_type == GRAD ? (asinl(x) * 200 / PI) : (asinl(x));
		else
			return Error = ERROR::domUndef, 0;
	}
	else if (op == Operator::H_acos)
	{
		if (x <= 1.0 && x >= -1.0)
			ans = angle_type == DEG ? (acosl(x) * 180 / PI) :
				angle_type == GRAD ? (acosl(x) * 200 / PI) : (acosl(x));
		else
			return Error = ERROR::domUndef, 0;
	}
	else if (op == Operator::H_atan)
		ans = angle_type == DEG ? (atanl(x) * 180 / PI) :
			angle_type == GRAD ? (atanl(x) * 200 / PI) : (atanl(x));
	else if (op == Operator::H_acosec)
	{
		if (x <= -1.0 || x >= 1.0)
			ans = angle_type == DEG ? (asinl(1 / x) * 180 / PI) :
				angle_type == GRAD ? (asinl(1 / x) * 200 / PI) : (asinl(1 / x));
		else
			return Error = ERROR::domUndef, 0;
	}
	else if (op == Operator::H_asec)
	{
		if (x <= -1 || x >= 1)
			ans = angle_type == DEG ? (acosl(1 / x) * 180 / PI) :
				angle_type == GRAD ? (acosl(1 / x) * 200 / PI) : (acosl(1 / x));
		else
			return Error = ERROR::domUndef, 0;
	}
	else if (op == Operator::H_acot)
		ans = angle_type == DEG ? (atanl(1 / x) * 180 / PI) :
			angle_type == GRAD ? (atanl(1 / x) * 200 / PI) : (atanl(1 / x));

	/* Logical operators */
	else if ((x == 1 || !x) && (y == 1 || !y))
	{
		if (op == Operator::H_not)       ans = !x;
		else if (op == Operator::H_or)   ans = x || y;
		else if (op == Operator::H_and)  ans = x && y;
	}
	else
		return Error = ERROR::invalidOptr, 0;

	this->numberStack.push(ans);
	return this->operatorStack.get(op);
}

#endif
