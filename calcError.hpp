#ifndef calcERROR
#define calcERROR

#include "common.hpp"

#ifdef ERROR
#define TEMP_ERROR ERROR
#undef ERROR
#endif

#ifdef DEBUG
#define error(err_type) {						\
    std::cerr << std::endl << "Error: '" << Error.toString() << "',";	\
    std::cerr << " File: '" << __FILE__ << "', Line: " << __LINE__;	\
    throw new ERROR(ERROR::err_type);					\
  }
#else
#define error(err_type)  throw new ERROR(ERROR::err_type);
#endif

class ERROR {
private:
  signed char e;

public:
  ERROR();
  explicit ERROR(const signed char);
  enum {
    noError = 0,
    memAlloc = -1,
    outOfRange = -2,
    numScarce = -3,
    optrScarce = -4,
    brktError = -5,
    divError = -6,
    factError = -7,
    rangUndef = -8,
    domUndef = -9,
    invalidOptr = -10,
    parseError = -11,
    invalidAns = -12,
    invalidCmd = -13,
    sizeError = -14
  };
  constStr toString() const;
  bool isSet() const;
  void set(const signed char);
  void reset();
  const ERROR operator=(int);
};

#ifdef TEMP_ERROR
#define ERROR TEMP_ERROR
#undef TEMP_ERROR
#endif

#endif
