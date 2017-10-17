#ifndef CALC_DEBUG_H
#define CALC_DEBUG_H

#ifdef DEBUG

#include <iostream>
#include <errno.h>


#define START_DEBUG {						\
  std::cerr << __FILE__ << ":" << __LINE__ << std::endl;

#define _CALC_DEBUG_   std::cerr << "\t"
#define DEBUG_VAR(var) _CALC_DEBUG_ << #var << " = " << var << std::endl
#define DEBUG_var(var) _CALC_DEBUG_ << #var << " = " << var
#define DEBUG_MSG(msg) _CALC_DEBUG_ << msg << std::endl
#define DEBUG_msg(msg) _CALC_DEBUG_ << msg
#define DEBUG_ERRNO    _CALC_DEBUG_ << strerror(errno) << std::endl
#define DEBUG_ERR(err) _CALC_DEBUG_ << "Error: " << err << std::endl
#define DEBUG_err(err) _CALC_DEBUG_ << "Error: " << err

#define END_DEBUG }

#endif // DEBUG

#endif // CALC_DEBUG_H
