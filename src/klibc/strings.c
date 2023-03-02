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

void strrev(char *s) {
  char c;

  for (int i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

char *utoa(char *str, unsigned int value, unsigned char base) {
  const char base_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  unsigned int i = 0;

  str[0] = '\0';
  if (base < 2 || base > 36)
    return NULL;
  while (value != 0) {
    str[i++] = base_digits[value % base];
    value /= base;
  }
  str[i] = '\0';
  strrev(str);
  return str;
}