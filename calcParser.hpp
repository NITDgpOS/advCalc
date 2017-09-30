#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "answerManager.hpp"
#include "calcOptr.hpp"
#include "common.hpp"
#include "str.hpp"

template <typename numT> class calcParse {
  constStr input;
  constStr currentPos;
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
  void gotCloseBracket();
  void gotPlusMinus();
  void gotChar();
  void gotNum();
  void gotOptr(const Operator &);
  void gotAns();
  bool gotVar();

  inline bool isOpenBracket() { return *this->currentPos == '('; }

  inline bool isAns() {
    return *this->currentPos == 'a' && isdigit(this->currentPos[1]);
  }

  inline bool isCloseBracket() { return *this->currentPos == ')'; }

  inline bool isPlus() { return *this->currentPos == '+'; }

  inline bool isMinus() { return *this->currentPos == '-'; }

  inline bool isChar() { return isalpha(*this->currentPos); }

  inline bool isNum() {
    return isdigit(*this->currentPos) ||
           (*this->currentPos == '.' && isdigit(this->currentPos[1]));
  }

public:
  calcParse()
      : input(NULL), currentPos(NULL), ans(0), end(0), running(false),
        over(false), prevToken(ClearField) {}
  explicit calcParse(constStr inp)
      : input(inp), currentPos(NULL), ans(0), end(0), running(false),
        over(false), prevToken(ClearField) {}
  calcParse(str inp, str start)
      : input(inp), currentPos(start), ans(0), end(0), running(false),
        over(false), prevToken(ClearField) {}
  calcParse(const calcParse &x) { *this = x; }
  bool isParsing() { return running; }
  bool isOver() { return over; }
  void startParsing();
  numT Ans() { return ans; }
  calcParse<numT> &operator=(const calcParse &x) {
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

template <typename numT> void calcParse<numT>::gotCloseBracket() {
  this->prevToken = CloseBracket;
  ++this->currentPos;
  Operator top;
  while (optr.operatorStack.pop(top) && top != Operator::H_openBracket) {
    optr.calculate(top);
  }
  if (top != Operator::H_openBracket)
    error(brktError);
}

template <typename numT> void calcParse<numT>::gotNum() {
  if (this->prevToken == CloseBracket)
    this->optr.insertOptr(Operator::H_multiply);
  this->prevToken = Number;
  numT x = 0;
  if (strToNum(&this->currentPos, x, REAL) == 0)
    error(parseError);
  optr.insertNum(x);
}

template <typename numT> void calcParse<numT>::gotOptr(const Operator &op) {
  this->prevToken = op.isUnary() ? UnaryOperator : BinaryOperator;
  this->optr.insertOptr(op);
}

template <typename numT> void calcParse<numT>::gotAns() {
  numT number;
  answers.parseAns(this->currentPos, number);
  if (this->prevToken == CloseBracket)
    this->optr.insertOptr(Operator::H_multiply);
  this->prevToken = Number;
  optr.insertNum(number);
}

template <typename numT> void calcParse<numT>::gotChar() {
  Operator op;
  if (this->isAns())
    this->gotAns();
  else if (op.parse(this->currentPos))
    this->gotOptr(op);
  else
    error(parseError);
}

template <typename numT> void calcParse<numT>::gotPlusMinus() {
  if (this->prevToken == Number or this->prevToken == CloseBracket) {
    this->prevToken = BinaryOperator;
    this->optr.insertOptr(this->isPlus() ? Operator::H_plus
                                         : Operator::H_minus);
    this->currentPos++;
  } else
    this->gotNum();
}

template <typename numT> void calcParse<numT>::startParsing() {
  this->running = true;

  prevToken = ClearField;

#ifdef TESTING
  if (this->input)
    throw "calcParse<T>::input not set\n";
#endif

  if (this->currentPos == NULL)
    this->currentPos = this->input;

  if (*this->currentPos == '#')
    error(noError);

  while (*this->currentPos && *this->currentPos != end) {

    skipSpace(this->currentPos);

    if (*this->currentPos == '#') {
      break;
    }

    if (this->isOpenBracket()) {
      this->gotOpenBracket();
    } else if (this->isCloseBracket()) {
      this->gotCloseBracket();
    } else if (this->isNum()) {
      this->gotNum();
    } else if (this->isPlus() or this->isMinus()) {
      this->gotPlusMinus();
    } else
      this->gotChar();
  }

  optr.finishCalculation();

  if (optr.ans(this->ans)) {
    answers.push(this->ans);
  }

  this->running = false;
  this->over = true;
}

#endif
