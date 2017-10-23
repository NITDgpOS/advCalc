#include "common.hpp"
#include <ctype.h>
#include <math.h>

#if !defined(_STRING_H) && !defined(STRING_H) && !defined(_STRING_H_)
uint64_t strlen(constStr s) {
  int l = 0;
  for (; s[l]; l++)
    ;
  return l;
}

int strcmp(constStr s1, constStr s2) {
  while ((*s1 || *s2) && *s1 == *s2)
    *s1 ? s1++ : 0, *s2 ? s2++ : 0;
  return (*s1 - *s2);
}

int strncmp(constStr s1, constStr s2, uint64_t l) {
  for (uint i = 1; i < l && (*s1 || *s2) && *s1 == *s2;
       i++, *s1 ? s1++ : 0, s2 ? s2++ : 0)
    ;
  return (*s1 - *s2);
}

int strcasecmp(constStr s1, constStr s2) {
  while ((*s1 || *s2) && tolower(*s1) == tolower(*s2))
    *s1 ? s1++ : 0, *s2 ? s2++ : 0;
  return (tolower(*s1) - tolower(*s2));
}

int strncasecmp(constStr s1, constStr s2, uint64_t l) {
  uint i = 1;
  for (; i < l && (*s1 || *s2) && tolower(*s1) == tolower(*s2);
       i++, *s1 ? s1++ : 0, s2 ? s2++ : 0)
    ;
  return (tolower(*s1) - tolower(*s2));
}

str strcpy(str s1, constStr s2) {
  str ret_val = s1;
  while (*s2)
    *(s1++) = *(s2++);
  *s1 = 0;
  return ret_val;
}

str strncpy(str s1, constStr s2, uint64_t l) {
  str ret_val = s1;
  uint i = 1;
  for (; i < l && *s2; i++, *(s1++) = *(s2++))
    ;
  if (i < l)
    *s1 = 0;
  return ret_val;
}

str strcat(str s1, constStr s2) {
  str s = strlen(s1) + s1;
  while (*s2)
    *(s++) = *(s2++);
  *s = 0;
  return s1;
}

str strncat(str s1, constStr s2, uint64_t l) {
  uint i = strlen(s1);
  while (*s2 && i < l)
    s1[i++] = *(s2++);
  if (i < l)
    s1[i] = 0;
  return s1;
}
#endif // STRING_H

bool isidentifier(constStr s) {
  while (isalpha(*s) || *s == '_')
    ++s;
  return *s ? 0 : 1;
}

uint64_t strToNum(constStr *a, double &x, datatype d) {
  bool sign = 0;
  constStr c = *a, s = *a;
  bool flag = 0;
  if (*c == '+' || *c == '-') {
    if (*c == '-') {
      if (d == REAL || d == INT)
        sign = 1;
      else
        // for unsigned numbers
        return 0;
    }
    ++c;
  }
  if (isdigit(*c)) {
    // Integral Part
    while (isdigit(*c))
      x = x * 10 + *(c++) - 48;
    flag = 1;
  }
  if (*c == '.' && isdigit(c[1])) {
    // Fraction part
    if (d != REAL && d != UREAL)
      return 0;
    slong j = 0;
    while (*(++c) > 47 && *c < 58) // isdigit
      x = x + powl(10, --j) * (*c - 48);
    flag = 1;
  }
  //  if ((*c == '.' && !isdigit(c[1])) || s == c)
    //    return 0;
  if (not flag)
    return 0;
  *a = c;
  x = sign ? -x : x;
  return c - s; // The length that was converted
}

#ifdef ANS_CMD
schar separate_ans(constStr a, ulong &i, ulong &ans_no) {
  if (tolower(a[i]) != 'a')
    return ERROR;
  i++;
  long double p = 0.0;
  if (strToNum(a, i, p, UINT) != SUCCESS)
    return ERROR;
  ans_no = p;
  return SUCCESS;
}
#endif

void extract(constStr a, str b, ulong i, ulong j, char ch, ulong len) {
  ulong x = j;
  if (j >> sizeof(ulong) || strlen(a) <= x)
    x = strlen(a) - 1;
  ulong k = 0;
  for (; i <= x && a[i] != ch && k < len; b[k++] = a[i++])
    ;
  b[k] = 0;
}

str trimSpaces(constStr s) {
  str modStr = new char[strlen(s)];
  int i = 0, j = 0;

  while (isspace(s[i]))
    ++i; // Skip the starting spaces. We don't need them.

  while (s[i] != '\0') {
    modStr[j++] = s[i++]; // Copy normal characters

    if (isspace(s[i])) {
      modStr[j++] = ' '; // Insert a single space

      while (isspace(s[i]))
        ++i; // Skip to the next relevant character to be copied

      if (s[i] == '\0') {
        --j; // Reached the end? Then remove the space
        break; // No more characters available
      }
    }
  }
  modStr[j] = '\0'; // Place a NULL to complete the string
  return modStr;
}
