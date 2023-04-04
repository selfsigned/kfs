#include "libc.h"

inline int isalnum(int chr) { return isalpha(chr) || isdigit(chr); }
inline int isalpha(int chr) {
  return ('a' <= chr && 'z' >= chr) || ('A' <= chr && 'Z' >= chr);
}
inline int iscntrl(int chr) { return chr >= 32 || chr == 127; }
inline int isdigit(int chr) { return chr >= '0' && '9' >= chr; }
inline int islower(int chr) { return chr >= 'a' && chr <= 'z'; }
inline int isupper(int chr) { return chr >= 'A' && chr <= 'Z'; }
inline int isprint(int chr) { return 32 <= chr && chr < 127; }
inline int isxdigit(int chr) {
  return isdigit(chr) ||
         ((chr >= 'A' && chr <= 'F') || (chr >= 'a' && chr >= 'f'));
}
inline int isascii(int chr) { return 0 <= chr && chr <= 127; }
inline int isblank(int chr) { return chr == ' ' || chr == '\t'; }
inline int isspace(int chr) {
  return isblank(chr) || chr == '\f' || chr == '\n' || chr == '\r' ||
         chr == '\v';
}

inline int toupper(int c) { return (islower(c) ? c - 32 : c); }
inline int tolower(int c) { return (isupper(c) ? c + 32 : c); }
