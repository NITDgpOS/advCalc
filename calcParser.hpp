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

public:
  calcParse()
      : input(NULL), currentPos(NULL), ans(0), end(0), running(false),
        over(false) {}
  explicit calcParse(str inp)
      : input(inp), currentPos(NULL), ans(0), end(0), running(false),
        over(false) {}
  calcParse(str inp, str start)
      : input(inp), currentPos(start), ans(0), end(0), running(false),
        over(false) {}
  calcParse(const calcParse &x) { *this = x; }
  bool isParsing() { return running; }
  bool isOver() { return over; }
  bool startParsing();
  numT Ans() { return ans; }
  void operator=(const calcParse &x) {
    this->input = x.input;
    this->currentPos = x.currentPos;
    this->ans = x.ans;
    this->end = x.end;
    this->running = x.running;
    this->over = x.over;
  }
};

template <typename numT> bool calcParse<numT>::startParsing() {
  this->running = true;
  operatorManager<numT> optr;

#ifdef TESTING
  if (this->input)
    throw "calcParse<T>::input not set\n";
#endif

  if (this->currentPos == NULL)
    this->currentPos = this->input;

  while (*this->currentPos && *this->currentPos != end) {

    //optr.operatorStack.display("", " ");

    while (isspace(*this->currentPos)) // Skip spaces
      ++this->currentPos;

    if (*this->currentPos == '#') // A comment
      break;

    if (*this->currentPos == '(' && this->currentPos[1] == '-') {
      Operator op1(Operator::H_openBracket);
      Operator op2(Operator::H_minus);
      optr.insert(op1);
      optr.insert(0);
      optr.insert(op2);
      this->currentPos += 2;
    }

    if (isdigit(*this->currentPos) || *this->currentPos == '.') {
      numT x = 0;
      if (strToNum(&this->currentPos, x, UREAL) == 0)
        return error(parseError);
      optr.insert(x);
    } else {
      Operator op;
      if (op.parse(&this->currentPos)) {
        if (!optr.insert(op))
          return 0;
      } else
        return error(parseError);
    }
  }

  optr.finishCalculation();

  optr.ans(this->ans);

  this->running = false;
  this->over = true;

  return 1;
}

#endif
