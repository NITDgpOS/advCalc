#ifndef CALC_STR_H
#define CALC_STR_H

#include "common.hpp"

#define SKIP_SPACE(s, i) s[i] == ' ' ? ++i : i

extern uint64_t strlen(const char *s);
extern int strcmp(const char *s1, const char *s2);
extern int strncmp(const char *s1, const char *s2, uint64_t l);
extern int strcasecmp(const char *s1, const char *s2);
extern int strncasecmp(const char *s1, const char *s2, uint64_t l);
extern char* strcpy(char *s1, const char *s2);
extern char* strncpy(char *s1, const char *s2, uint64_t l);
extern char* strcat(char *s1, const char *s2);
extern char* strncat(char *s1, const char *s2, uint64_t l);

extern bool isidentifier(const char *s);

extern signed char strToNum(char **a, double &x, datatype d);

#ifdef ANS_CMD
extern signed char separate_ans(const char *a, unsigned long &i, unsigned long &x);
#endif

#endif // CALC_STR_H
