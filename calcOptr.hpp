#ifndef calcOPTR
#define calcOPTR

#include "calcError.hpp"
#include "calcStack.hpp"
#include <math.h>

typedef ulong str_hash;
typedef uint optr_hash;

extern void makeOperatorHashes();
extern double factorial(double x);

class Operator {
public:
  enum optrHash {
    H_plus = 43,
    H_minus = 45,
    H_multiply = 42,
    H_divide = 47,
    H_pow = 94,
    H_mod = 37,
    H_P = 80,
    H_C = 67,
    H_bitAnd = 38,
    H_bitOr = 124,
    H_bitShiftRight = 7936,
    H_bitShiftLeft = 7680,
    H_bitNot = 126,
    H_openBracket = 40,
    H_closeBracket = 41,
    H_not = 33,
    H_and = 4864,
    H_or = 15872,
    H_great = 62,
    H_less = 60,
    H_greatEqual = 7935,
    H_lessEqual = 7681,
    H_equal = 7808,
    H_notEqual = 4252,
    H_sin = 1868280,
    H_cos = 1610983,
    H_tan = 1883393,
    H_sec = 1867761,
    H_cosec = 983557758,
    H_cot = 1610984,
    H_asin = 200561431,
    H_acos = 200304134,
    H_atan = 200576544,
    H_asec = 200560912,
    H_acosec = 705393442,
    H_acot = 200304135,
    H_sinh = 237271664,
    H_cosh = 204594945,
    H_tanh = 239191015,
    H_log = 1756132,
    H_ln = 13826,
    H_logten = 232822054,
    H_abs = 1577074,
    H_floor = 757783302,
    H_ceil = 204432389
  };

private:
  optr_hash op;
  bool isBinary;
  uint8_t priority;
  bool setOperatorProperties();

public:
  Operator();
  explicit Operator(optr_hash);
  explicit Operator(constStr );
  Operator(const Operator &);
  bool isUnary() const;
  constStr toString() const;
  uint8_t checkPriority(const Operator) const;
  uint8_t parse(str &);
  bool setFromString(constStr );
  bool isBracket();
  void operator=(const Operator &);
  bool operator==(const Operator::optrHash) const;
  bool operator==(const Operator) const;
  bool operator!=(const Operator::optrHash) const;
  bool operator!=(const Operator) const;
  bool operator<(const Operator) const;
  bool operator>(const Operator) const;
  std::ostream &operator<<(std::ostream &out) const {
    return (out << this->toString());
  }
  operator constStr () { return this->toString(); }
  operator int64_t() { return this->op; }
  operator optr_hash() { return this->op; }
  operator uint8_t() { return priority; }
};

#define RAD 0
#define DEG 1
#define GRAD 2

#define PI 3.14

extern unsigned char angle_type;

template <typename numType> class operatorManager {
  calcStack<Operator> operatorStack;
  calcStack<numType> numberStack;
  // Calculates the ans and puts it into the numberStack
  template <typename num> friend class calcParse;
  void calculate(const Operator &);

public:
  // Insert a given Operator into the operatorStack. Uses calculate().
  void insertOptr(const Operator);
  // Insert a given Operator given the optrHash
  void insertOptr(const Operator::optrHash oh) {
    Operator top(oh);
    this->insertOptr(top);
  }
  // Push a number into the numberStack
  void insertNum(const numType);
  // No more input left. Pop out and calculate everything left.
  bool finishCalculation();
  // Pop out the last number in the numberStack
  bool ans(numType &);

  template <typename numT> friend class calcParse;
};

template <typename numType>
void operatorManager<numType>::insertOptr(const Operator z) {
  Operator top;

  // Check(top > z) only if any operator is returned into top
  if (not this->operatorStack.get(top) or top > z) {
    this->operatorStack.push(z);
    return;
  }

  do {
    this->operatorStack.pop(); // The operator(top)
    this->calculate(top);      // Calculate the result
  } while (this->operatorStack.get(top) && top < z);

  this->operatorStack.push(z);
}

template <typename numType>
void operatorManager<numType>::insertNum(const numType x) {
  this->numberStack.push(x);
}

template <typename numType> bool operatorManager<numType>::finishCalculation() {
  Operator top;

  while (this->operatorStack.pop(top)) {
    if (top != Operator::H_openBracket)
      this->calculate(top);
  }

  if (this->operatorStack.isEmpty())
    return 1;
  error(numScarce);
}

template <typename numType> bool operatorManager<numType>::ans(numType &x) {
#ifdef TESTING
  if (not this->operatorStack.isEmpty())
    throw "Operators left in stack due to some error";
#endif
  if (this->numberStack.pop(x) && this->numberStack.isEmpty())
    return 1;
  error(numScarce);
}

template <typename numType>
void operatorManager<numType>::calculate(const Operator &top) {
  numType x = 0, y = 0;
  if (not this->numberStack.pop(y))
    error(numScarce);
  if (not top.isUnary() && not this->numberStack.pop(x))
    // The second number iff top is a binary operator
    error(numScarce);

  numType z = angle_type == DEG ? (y * PI / 180)
                                : (angle_type == RAD ? y : (y * PI / 200)),
          ans;

  /* Basic arithmatic operators */
  if (top == Operator::H_plus)
    ans = x + y;
  else if (top == Operator::H_minus)
    ans = x - y;
  else if (top == Operator::H_multiply)
    ans = x * y;
  else if (top == Operator::H_divide) {
    if (y)
      ans = x / y;
    else
      error(divError);
  } else if (top == Operator::H_pow)
    ans = powl(x, y);

  /* Factorials */
  else if (top == Operator::H_P) {
    if (x >= 0 && y >= 0 && x >= y && !(x - floorl(x)) && !(y - floorl(y)))
      ans = factorial(x) / factorial(x - y);
    else
      error(factError);
  } else if (top == Operator::H_C) {
    if (x >= 0 && y >= 0 && x >= y && !(x - floorl(x)) && !(y - floorl(y)))
      ans = factorial(x) / (factorial(y) * factorial(x - y));
    else
      error(factError);
  }

  /* Computer related basic operators */
  else if (top == Operator::H_bitNot)
    ans = ~(ulong)y;
  else if (top == Operator::H_bitOr)
    ans = (ulong)x | (ulong)y;
  else if (top == Operator::H_bitAnd)
    ans = (ulong)x & (ulong)y;
  else if (top == Operator::H_mod)
    ans = fmodl(x, y);
  else if (top == Operator::H_bitShiftRight)
    ans = (ulong)x >> (ulong)y;
  else if (top == Operator::H_bitShiftLeft)
    ans = (ulong)x << (ulong)y;

  /* Relational operators */
  else if (top == Operator::H_great)
    ans = x > y;
  else if (top == Operator::H_less)
    ans = x < y;
  else if (top == Operator::H_greatEqual)
    ans = x >= y;
  else if (top == Operator::H_lessEqual)
    ans = x <= y;
  else if (top == Operator::H_notEqual)
    ans = x != y;
  else if (top == Operator::H_equal)
    ans = x == y;

  /* Other mathematical functions */
  else if (top == Operator::H_log) {
    if (y > 0 && x >= 0)
      ans = logl(y) / logl(x);
    else
      error(rangUndef);
  } else if (top == Operator::H_abs)
    ans = fabsl(y);
  else if (top == Operator::H_ceil)
    ans = ceill(y);
  else if (top == Operator::H_floor)
    ans = floorl(y);
  else if (top == Operator::H_ln) {
    if (x > 0)
      ans = logl(y);
    else
      error(rangUndef);
  } else if (top == Operator::H_logten) {
    if (x > 0)
      ans = log10l(y);
    else
      error(rangUndef);
  } else if (top == Operator::H_sinh)
    ans = sinhl(z);
  else if (top == Operator::H_cosh)
    ans = coshl(z);
  else if (top == Operator::H_tanh)
    ans = tanhl(z);
  else if (top == Operator::H_sin)
    ans = sinl(z);
  else if (top == Operator::H_cos)
    ans = cosl(z);
  else if (top == Operator::H_tan) {
    if (cosl(z))
      ans = tanl(z);
    else
      error(rangUndef);
  } else if (top == Operator::H_cosec) {
    if (sinl(z))
      ans = 1 / sinl(z);
    else
      error(rangUndef);
  } else if (top == Operator::H_sec) {
    if (cosl(z))
      ans = 1 / cosl(z);
    else
      error(rangUndef);
  } else if (top == Operator::H_cot) {
    if (sinl(z))
      ans = 1 / tanl(z);
    else
      error(rangUndef);
  } else if (top == Operator::H_asin) {
    if (y <= 1 && y >= -1)
      ans = angle_type == DEG
                ? (asinl(y) * 180 / PI)
                : angle_type == GRAD ? (asinl(y) * 200 / PI) : (asinl(y));
    else
      error(domUndef);
  } else if (top == Operator::H_acos) {
    if (y <= 1.0 && y >= -1.0)
      ans = angle_type == DEG
                ? (acosl(y) * 180 / PI)
                : angle_type == GRAD ? (acosl(y) * 200 / PI) : (acosl(y));
    else
      error(domUndef);
  } else if (top == Operator::H_atan)
    ans = angle_type == DEG
              ? (atanl(y) * 180 / PI)
              : angle_type == GRAD ? (atanl(y) * 200 / PI) : (atanl(y));
  else if (top == Operator::H_acosec) {
    if (y <= -1.0 || y >= 1.0)
      ans = angle_type == DEG ? (asinl(1 / y) * 180 / PI)
                              : angle_type == GRAD ? (asinl(1 / y) * 200 / PI)
                                                   : (asinl(1 / y));
    else
      error(domUndef);
  } else if (top == Operator::H_asec) {
    if (y <= -1 || y >= 1)
      ans = angle_type == DEG ? (acosl(1 / y) * 180 / PI)
                              : angle_type == GRAD ? (acosl(1 / y) * 200 / PI)
                                                   : (acosl(1 / y));
    else
      error(domUndef);
  } else if (top == Operator::H_acot)
    ans = angle_type == DEG
              ? (atanl(1 / y) * 180 / PI)
              : angle_type == GRAD ? (atanl(1 / y) * 200 / PI) : (atanl(1 / y));

  /* Logical operators */
  else if ((x == 1 || x == 0) && (y == 1 || y == 0)) {
    if (top == Operator::H_not)
      ans = !y;
    else if (top == Operator::H_or)
      ans = x || y;
    else if (top == Operator::H_and)
      ans = x && y;
    else
      error(invalidOptr);
  } else
    error(invalidOptr);

  this->numberStack.push(ans);
}

#endif
