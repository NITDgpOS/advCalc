#ifndef CALC_COMMON_H
#define CALC_COMMON_H

#include <cstdint>
#include <cstdarg>
#include <memory>
#include <iostream>

/* Integer types */
typedef signed char schar;
typedef unsigned char uchar;
typedef signed int sint;
typedef unsigned int uint;
typedef signed long slong;
typedef unsigned long ulong;
typedef signed long long sll;
typedef unsigned long long ull;

/* Float types */
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;

typedef unsigned char datatype;

/* String types */
typedef char * str;
typedef const char * constStr;
typedef str charPtr;

#define REAL 0
#define INT 1
#define UREAL 2
#define UINT 3

#ifndef NULL
#define NULL 0UL
#endif

const bool DONT_PRINT = 0;
extern bool isQuiet;

#define Printf(...) (isQuiet ? DONT_PRINT : printf(__VA_ARGS__))

#define Println(...) {                          \
    Printf(__VA_ARGS__);                        \
    isQuiet ? DONT_PRINT :                      \
      bool(std::cout << std::endl);             \
  }

#define println(...) {                          \
    printf(__VA_ARGS__);                        \
    std::cout << std::endl;                     \
  }

#endif // CALC_COMMON_H
