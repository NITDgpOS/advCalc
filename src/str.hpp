#ifndef CALC_STR_H
#define CALC_STR_H

#include "common.hpp"
#include <ctype.h>

inline void skipSpace(constStr s, ulong &i) {
  isspace(s[i]) ? ++i : i;
}

inline void skipSpace(constStr &s) {
  isspace(*s) ? ++s : s;
}

#if !defined(_STRING_H) && !defined(STRING_H) && !defined(_STRING_H_)
extern uint64_t strlen(constStr s);
extern int strcmp(constStr s1, constStr s2);
extern int strncmp(constStr s1, constStr s2, uint64_t l);
extern int strcasecmp(constStr s1, constStr s2);
extern int strncasecmp(constStr s1, constStr s2, uint64_t l);
extern char* strcpy(str s1, constStr s2);
extern char* strncpy(str s1, constStr s2, uint64_t l);
extern char* strcat(str s1, constStr s2);
extern char* strncat(str s1, constStr s2, uint64_t l);
#endif

extern bool isidentifier(constStr s);

extern signed char strToNum(constStr *a, double &x, datatype d);

extern str trimSpaces(constStr s);

#endif // CALC_STR_H
