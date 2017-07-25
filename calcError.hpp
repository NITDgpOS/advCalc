#ifndef calcERROR
#define calcERROR

#include "common.hpp"

#ifdef ERROR
#define TEMP_ERROR ERROR
#undef ERROR
#endif

#ifdef DEBUG
#define _error(err_type, ret_val)                                              \
  ((Error = ERROR::err_type),                                                  \
   (std::cerr << "Error: '" << Error.toString() << "', File: '" << __FILE__    \
              << "', Line: " << __LINE__ << std::endl),                        \
   ret_val)
#else
#define _error(err_type, ret_val) ((Error = ERROR::err_type), (ret_val))
#endif
#define error(err_type) _error(err_type, 0)

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
    invalidCmd = -13
  };
  constStr toString() const;
  bool isSet() const;
  void set(const signed char);
  void reset();
  const ERROR operator=(int);
};

extern ERROR Error;

#ifdef TEMP_ERROR
#define ERROR TEMP_ERROR
#undef TEMP_ERROR
#endif

#endif

/*
  Local Variables:
  mode: c++
  c-file-offset: 2
  eval: (clang-format-buffer)
  fill-column: 80
  End:
*/
