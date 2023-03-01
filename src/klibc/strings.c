#include "libc.h"
#include <stdbool.h>
#include <stddef.h>

#define __null 0

// memory

inline void *memset(void *s, int c, size_t n) {
  for (size_t i = 0; i < n; ++i)
    ((char *)s)[i] = c;
  return s;
}

inline void bzero(void *s, size_t n) { memset(s, 0, n); }

// strings

inline size_t strlen(const char *s) {
  const char *t;

  for (t = s; *t; ++t)
    ;
  return t - s;
}

char *strchr(const char *s, int c) {
  while (*s) {
    if (*s == c)
      return (char *)s;
    s++;
  }
  return NULL;
}

int atoi(const char *str) {
  long int r = 0;
  bool neg = false;

  for (; isspace(*str); ++str)
    ;
  neg = (*str == '-');
  if (neg || *str == '+')
    str++;
  for (; isdigit(*str); ++str)
    r = r * 10 + (*str - '0');
  return (neg) ? -r : r;
}

// non-standard
