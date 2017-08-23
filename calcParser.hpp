#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "calcOptr.hpp"
#include "common.hpp"
#include "str.hpp"

template <typename numT> class calcParse {
  str input;
  str currentPos;
  numT ans;
  char end;
  bool running;
  bool over;
  typedef enum {
    ClearField,
    Number,
    BinaryOperator,
    UnaryOperator,
    OpenBracket,
    CloseBracket
  } prevTokenType;
  prevTokenType prevToken;
  operatorManager<numT> optr;

  void gotOpenBracket();
  bool gotCloseBracket();
  bool gotPlusMinus();
  bool gotChar();
  bool gotNum();

  inline bool isOpenBracket() {
    return *this->currentPos == '(';
  }

  inline bool isCloseBracket() {
    return *this->currentPos == ')';
  }

  inline bool isPlus() {
    return *this->currentPos == '+';
  }

  inline bool isMinus() {
    return *this->currentPos == '-';
  }

  inline bool isChar() {
    return isalpha(*this->currentPos);
  }

  inline bool isNum() {
    return *this->currentPos == '.' || isdigit(*this->currentPos);
  }

public:
  calcParse()
      : input(NULL), currentPos(NULL), ans(0), end(0), running(false),
        over(false), prevToken(ClearField) {}
  explicit calcParse(str inp)
      : input(inp), currentPos(NULL), ans(0), end(0), running(false),
        over(false), prevToken(ClearField) {}
  calcParse(str inp, str start)
      : input(inp), currentPos(start), ans(0), end(0), running(false),
        over(false) , prevToken(ClearField){}
  calcParse(const calcParse &x) { *this = x; }
  bool isParsing() { return running; }
  bool isOver() { return over; }
  bool startParsing();
  numT Ans() { return ans; }
  calcParse<numT>& operator=(const calcParse &x) {
    this->input = x.input;
    this->currentPos = x.currentPos;
    this->ans = x.ans;
    this->end = x.end;
    this->running = x.running;
    this->over = x.over;
    return *this;
  }
};

template <typename numT> void calcParse<numT>::gotOpenBracket() {
  if (this->prevToken == Number)
    this->optr.insertOptr(Operator::H_multiply);
  this->prevToken = OpenBracket;
  ++this->currentPos;
  Operator op(Operator::H_openBracket);
  this->optr.operatorStack.push(op);
}

template <typename numT> bool calcParse<numT>::gotCloseBracket() {
  this->prevToken = CloseBracket;
  ++this->currentPos;
  Operator top;
  while (optr.operatorStack.pop(top) && top != Operator::H_openBracket) {
    optr.calculate(top);
  }
  if (top != Operator::H_openBracket)
    return error(brktError);
  return 1;
}

template <typename numT> bool calcParse<numT>::gotNum() {
  if (this->prevToken == CloseBracket)
    this->optr.insertOptr(Operator::H_multiply);
  this->prevToken = Number;
  numT x = 0;
  if (strToNum(&this->currentPos, x, REAL) == 0)
    return error(parseError);
  optr.insertNum(x);
  return 1;
}

template <typename numT> bool calcParse<numT>::gotChar() {
  Operator op;
  if (op.parse(this->currentPos)) {
    this->prevToken = op.isUnary() ? UnaryOperator : BinaryOperator;
    if (not this->optr.insertOptr(op))
      return 0;
  } else
    return error(parseError);
  return 1;
}

template <typename numT> bool calcParse<numT>::gotPlusMinus() {
  if (this->prevToken == Number or this->prevToken == CloseBracket) {
    this->prevToken = BinaryOperator;
    this->optr.insertOptr(this->isPlus() ? Operator::H_plus : Operator::H_minus);
    this->currentPos++;
  } else if (not this->gotNum())
    return 0;
  return 1;
}

template <typename numT> bool calcParse<numT>::startParsing() {
  this->running = true;

  prevToken = ClearField;

#ifdef TESTING
  if (this->input)
    throw "calcParse<T>::input not set\n";
#endif

  if (this->currentPos == NULL)
    this->currentPos = this->input;

  while (*this->currentPos && *this->currentPos != end) {

    skipSpace(this->currentPos);

    if (*this->currentPos == '#') // A comment
      break;

    if (this->isOpenBracket()) {
      this->gotOpenBracket();
    } else if (this->isCloseBracket()) {
      if (not this->gotCloseBracket())
        return 0;
    } else if (this->isNum()) {
      if (not this->gotNum())
        return 0;
    } else if (this->isPlus() or this->isMinus()) {
      if (not gotPlusMinus())
        return 0;
    } else if (not this->gotChar())
        return 0;
  }

  optr.finishCalculation();

  optr.ans(this->ans);

  this->running = false;
  this->over = true;

  return 1;
}

#endif
