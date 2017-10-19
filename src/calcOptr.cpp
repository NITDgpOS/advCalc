#include <ctype.h>

#include "calcOptr.hpp"

uint8_t angle_type = DEG;

static const char bin_ops[22][4] = {
  "+", "-", "*",  "/",  "^",  "|",  "&", "P",   "C",  ">>", "<<",
  "<", ">", ">=", "<=", "==", "!=", "%", "log", "&&", "||", ""
};

static const char un_ops[22][7] = {
  "~",      "!",     "ln",   "abs",   "cos",  "sin",  "tan",    "acos",
  "asin",   "atan",  "sec",  "cosec", "cot",  "asec", "acosec", "acot",
  "logten", "floor", "sinh", "cosh",  "tanh", "ceil"
};

static optr_hash binOpsHash[22] = {0};
static optr_hash unOpsHash[22] = {0};

static const ulong unify = 999999999;

Operator::Operator() {
  this->op = this->priority = 0;
  this->isBinary = false;
}

Operator::Operator(optr_hash x) {
  this->op = x;
  this->setOperatorProperties();
}

Operator::Operator(constStr x) {
  if (not this->setFromString(x)) {
    this->op = this->priority = 0;
    this->isBinary = 0;
  }
}

Operator::Operator(const Operator &x) {
  this->op = x.op;
#ifdef TESTING
  this->setOperatorProperties();
  if (x.isBinary != this->isBinary || x.priority != this->priority)
    throw "Operator error caught\n"
      "Unable to Operator::setOperatorProperties\n"
      "Operators don't match" FILE "\n";
#else
  this->isBinary = x.isBinary;
  this->priority = x.priority;
#endif
}

bool Operator::isUnary() const {
#ifdef TESTING
  if (not this->op)
    throw "Error: Null operator beign checked for unary";
#endif
  return not this->isBinary;
}

void Operator::operator=(const Operator &x) {
  this->op = x.op;
#ifdef TESTING
  this->setOperatorProperties();
  if (x.isBinary != this->isBinary || x.priority != this->priority)
    throw "Operator error caught\n"
      "Unable to Operator::setOperatorProperties\n"
      "Operators don't match" FILE "\n";
#else
  this->isBinary = x.isBinary;
  this->priority = x.priority;
#endif
}

static str_hash generateHashKey(constStr s, ulong start = 0, ulong end = 0) {
  register constStr x = s = s + start;
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

double factorial(double x) {
  long double t = 1;
  for (long double i = 1; i <= x; i++)
    t *= i;
  return t;
}

bool ismathchar(const char ch) {
  return (isalpha(ch) || ch == '/' || ch == '*' || ch == '-' || ch == '+' ||
          ch == '^' || ch == '%' || ch == '_' || ch == '!' || ch == '&' ||
          ch == '|' || ch == '<' || ch == '>' || ch == '~' || ch == '=');
}

bool isBracket(const char ch) { return (ch == '(' || ch == ')'); }

static int priorityOf(const optr_hash s) {
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
  switch (s) {
  case Operator::H_and:
  case Operator::H_or:
    return 1;

  case Operator::H_great:
  case Operator::H_less:
  case Operator::H_equal:
  case Operator::H_notEqual:
  case Operator::H_greatEqual:
  case Operator::H_lessEqual:
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

  case Operator::H_bitAnd:
  case Operator::H_bitOr:
    return 7;

  case Operator::H_bitShiftRight:
  case Operator::H_bitShiftLeft:
    return 8;

  default:
    return 0;
  }
}

bool Operator::setOperatorProperties() {
  if (not op)
    return 0;
  if (this->isBracket()) {
    this->isBinary = false;
    this->priority = priorityOf(this->op);
    return 1;
  } else {
    uchar beg = 0, end = 20, mid;
    while (beg <= end) {
      mid = (beg + end) / 2;
      if (this->op == binOpsHash[mid] || this->op == binOpsHash[beg] ||
          this->op == binOpsHash[end]) {
        isBinary = true;
        this->priority = priorityOf(this->op);
        return 1;
      } else if (this->op < binOpsHash[mid])
        end = mid - 1, ++beg;
      else
        beg = mid + 1, --end;
    }
    beg = 0, end = 21;
    while (beg <= end) {
      mid = (beg + end) / 2;
      if (this->op == unOpsHash[mid] || this->op == unOpsHash[beg] ||
          this->op == unOpsHash[end]) {
        isBinary = false;
        this->priority = priorityOf(this->op);
        return 1;
      } else if (this->op < unOpsHash[mid])
        end = mid - 1, ++beg;
      else
        beg = mid + 1, --end;
    }
  }
  return 0;
}

// Parse the given string and return zero on error
uint8_t Operator::parse(constStr &start) {
  uint8_t charsRead = 0;
  str_hash hash = 0;
  Operator optr;
  for (uint i = 0; i < 6 && ismathchar(start[i]); ++i) {
    hash = 127 * hash + start[i];
    optr.op = hash % unify;
    if (optr.setOperatorProperties()) {
      // modify *this and charsRead iff optr is valid
      *this = optr;
      charsRead = i + 1;
    }
  }
  start += charsRead;
  return charsRead;
}

constStr Operator::toString() const {
  switch (this->op) {
  case H_plus:
    return "+";
  case H_minus:
    return "-";
  case H_multiply:
    return "*";
  case H_divide:
    return "/";
  case H_pow:
    return "^";
  case H_mod:
    return "%";
  case H_P:
    return "P";
  case H_C:
    return "C";
  case H_bitAnd:
    return "&";
  case H_bitOr:
    return "|";
  case H_bitShiftRight:
    return ">>";
  case H_bitShiftLeft:
    return "<<";
  case H_bitNot:
    return "~";
  case H_openBracket:
    return "(";
  case H_closeBracket:
    return ")";
  case H_not:
    return "!";
  case H_and:
    return "&&";
  case H_or:
    return "||";
  case H_great:
    return ">";
  case H_less:
    return "<";
  case H_greatEqual:
    return ">=";
  case H_lessEqual:
    return "<=";
  case H_sin:
    return "sin";
  case H_cos:
    return "cos";
  case H_tan:
    return "tan";
  case H_sec:
    return "sec";
  case H_cosec:
    return "cosec";
  case H_cot:
    return "cot";
  case H_asin:
    return "asin";
  case H_acos:
    return "acos";
  case H_atan:
    return "atan";
  case H_asec:
    return "asec";
  case H_acosec:
    return "acosec";
  case H_acot:
    return "acot";
  case H_sinh:
    return "sinh";
  case H_cosh:
    return "cosh";
  case H_tanh:
    return "tanh";
  case H_log:
    return "log";
  case H_ln:
    return "ln";
  case H_logten:
    return "logten";
  case H_abs:
    return "abs";
  case H_floor:
    return "floor";
  case H_ceil:
    return "ceil";
  case 0:
    return "(null)";
  default:
    return "";
  }
}

void makeOperatorHashes() {
  int i = 0, j;
  optr_hash t;

  /* Generating */
  for (; i < 22; ++i) {
    binOpsHash[i] = generateHashKey(bin_ops[i]) % unify;
    unOpsHash[i] = generateHashKey(un_ops[i]) % unify;
  }

  /* Sorting */
  for (i = 0; i < 22; ++i)
    for (j = 0; j < 22 - i - 1; ++j)
      if (unOpsHash[j] > unOpsHash[j + 1]) {
        t = unOpsHash[j];
        unOpsHash[j] = unOpsHash[j + 1];
        unOpsHash[j + 1] = t;
      }
  for (i = 0; i < 21; ++i)
    for (j = 0; j < 21 - i - 1; ++j)
      if (binOpsHash[j] > binOpsHash[j + 1]) {
        t = binOpsHash[j];
        binOpsHash[j] = binOpsHash[j + 1];
        binOpsHash[j + 1] = t;
      }
}

#define HIGH 20
#define LOW 10
// This function checks the priority of two operators and returns HIGH or LOW
// based on some conditions whether the operator should be poped out of stack or
// not.
uint8_t Operator::checkPriority(const Operator s2) const {
#ifdef TESTING
  if (not this->op || not s2.op)
    throw "Error: Null operator cannot have a priority";
#endif

  if (this->op == H_openBracket)
    return HIGH;

  if (this->isBinary && s2.isUnary())
    return HIGH;

  if (this->isUnary() && s2.isBinary)
    return LOW;

  if (this->isUnary() && s2.isUnary())
    return HIGH;

  if (this->op == s2.op) {
    if (this->isBinary)
      return LOW;
    return HIGH;
  }

  if (this->priority && s2.priority)
    return this->priority < s2.priority ? HIGH : LOW;
  return 0;
}

bool Operator::setFromString(constStr x) {
  this->op = generateHashKey(x, 0, 0);
  return this->setOperatorProperties();
}

bool Operator::isBracket() {
  return this->op == Operator::H_closeBracket ||
    this->op == Operator::H_openBracket;
}

bool Operator::operator==(const Operator::optrHash x) const {
  return this->op == x;
}

bool Operator::operator==(const Operator x) const { return this->op == x.op; }

bool Operator::operator!=(const Operator::optrHash x) const {
  return this->op != x;
}

bool Operator::operator!=(const Operator x) const { return this->op != x.op; }

bool Operator::operator>(const Operator x) const {
  return this->checkPriority(x) == HIGH;
}

bool Operator::operator<(const Operator x) const {
  return this->checkPriority(x) == LOW;
}
