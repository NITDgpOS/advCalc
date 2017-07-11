#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "common.hpp"
#include "str.hpp"
#include "calcOptr.hpp"

template <typename numT> class calcParse {
  char *input;
  char *currentPos;
  numT ans;
  char end;
  bool running;
  bool over;

public:
  calcParse()
      : input(NULL), currentPos(NULL), ans(0), end(0), running(false),
        over(false) {}
  explicit calcParse(char *inp)
      : input(inp), currentPos(NULL), ans(0), end(0), running(false),
        over(false) {}
  calcParse(char *inp, char *start)
      : input(inp), currentPos(start), ans(0), end(0), running(false),
        over(false) {}
  calcParse(const calcParse &x) { *this = x; }
  bool isParsing() { return running; }
  bool isOver() { return over; }
  void startParsing();
  void operator=(const calcParse &x) {
    this->input = x.input;
    this->currentPos = x.currentPos;
    this->ans = x.ans;
    this->end = x.end;
    this->running = x.running;
    this->over = x.over;
  }
};

template <typename numT>
void calcParse<numT>::startParsing() {
  this->running = true;
  operatorManager<numT> optr;

  if (this->currentPos == NULL)
    this->currentPos = this->input;

  while (*this->currentPos && *this->currentPos != end) {
    if (*this->currentPos == '(' && this->currentPos[1] == '-') {
      optr.insert(0);
      optr.insert(Operator::H_minus);
    }
    if (isdigit(*this->currentPos) || *this->currentPos == '.') {
      numT x = 0;
      if (strToNum(&this->currentPos, x, UREAL) == 0)
        return;
      optr.insert(x);
    } else {
      Operator op;
      if (op.parse(&this->currentPos))
        optr.insert(op);
    }
  }

  optr.finishCalculation();

  optr.ans(this->ans);

  this->running = false;
  this->over = true;
}

#endif
