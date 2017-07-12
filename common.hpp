#ifndef CALC_COMMON_H
#define CALC_COMMON_H

#include <stdint.h>

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

typedef uint8_t datatype;
#define REAL 0
#define INT 1
#define UREAL 2
#define UINT 3

#ifndef NULL
#define NULL 0UL
#endif

#endif // CALC_COMMON_H
