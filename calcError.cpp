#include "calcError.hpp"

const ERROR ERROR::operator=(signed int x) {
  e = x;
  return *this;
}

ERROR::ERROR() { this->e = 0; }

ERROR::ERROR(const signed char error) { this->e = error; }

constStr ERROR::toString() const {
  switch (this->e) {
  case noError:
    return "No error";
  case memAlloc:
    return "Unable to allocate memory";
  case outOfRange:
    return "Out of range";
  case numScarce:
    return "Number Scarcity error";
  case optrScarce:
    return "Operator Scarcity error";
  case brktError:
    return "Bracket Error";
  case divError:
    return "Divide Error";
  case rangUndef:
    return "Range Undefined";
  case domUndef:
    return "Domain Undefined";
  case invalidOptr:
    return "Invalid Operator";
  case parseError:
    return "Unable to parse expression";
  case invalidAns:
    return "Inavlid Answer";
  case invalidCmd:
    return "Invalid command";
  default:
    return "Undefined Error. Please report this event.";
  }
}

bool ERROR::isSet() const { return this->e; }

void ERROR::set(const signed char error) { this->e = error; }

void ERROR::reset() { this->e = noError; }
