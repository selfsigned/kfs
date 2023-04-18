#ifndef LIBC_H
#define LIBC_H

#include <stddef.h>
#include <stdint.h>

//// ctype.h ////

/// @brief  test for an alphanumeric character
/// @return non-zero if c is the thing, 0 if it isn't
int isalnum(int c);

/// @brief  test for an alphabetical character
/// @return non-zero if c is the thing, 0 if it isn't
int isalpha(int c);

/// @brief  test for a control character
/// @return non-zero if c is the thing, 0 if it isn't
int iscntrl(int c);

/// @brief  test for a digit (0-9)
/// @return non-zero if c is the thing, 0 if it isn't
int isdigit(int c);

/// @brief  test for lowercase
/// @return non-zero if c is the thing, 0 if it isn't
int islower(int c);

/// @brief  test if printable
/// @return non-zero if c is the thing, 0 if it isn't
int isprint(int c);

/// @brief  test for space (space, form-feed ('\f'), newline ('\n'), carriage
/// return ('\r'), horizontal tab ('\t'), and vertical tab ('\v'))
/// @return non-zero if c is the thing, 0 if it isn't
int isspace(int c);

/// @brief  test for uppercase
/// @return non-zero if c is the thing, 0 if it isn't
int isupper(int c);

/// @brief  test for hexadecimal digit
/// @return non-zero if c is the thing, 0 if it isn't
int isxdigit(int c);

/// @brief test for a 7-bit US-ASCII character
/// @return non-zero if c is the thing, 0 if it isn't
int isascii(int c);

/// @brief test for a blank character (tab or space)
/// @return non-zero if c is the thing, 0 if it isn't
int isblank(int c);

/// @brief convert uppercase
/// @param c char to convert
/// @return c converted to uppercase
int toupper(int c);

/// @brief convert lowercase
/// @param c char to convert
/// @return c converted to lowercase
int tolower(int c);

//// strings.h ////

/// @brief convert a string to an int
/// @param nptr pointer to a string that starts with a number
/// @return converted value or 0 on error
int atoi(const char *nptr);

/// @brief zero a byte string
/// @param s location to zero
/// @param n numbers of bytes to zero
void bzero(void *s, size_t n);

/// @brief copy memory area
/// @param dest destionation memory area (can't overlap with src)
/// @param src  src memory area
/// @param n number of bytes to copy
/// @return
void *memcpy(void *dest, const void *src, size_t n);

/// @brief fill memory with a constant byte
/// @param s memory area to fill
/// @param c character to fill
/// @param n number of bytes to fill
/// @return pointer to the memory area s
void *memset(void *s, int c, size_t n);

/// @brief returns the length of a string
/// @param s string to get the size of
/// @return size of the string
size_t strlen(const char *s);

/// @brief returns the length of a string up to maxlen
/// @param s string to get the size of
/// @param maxlen max num of character to read
/// @return strlen(s) if string is shorter than maxlen, otherwise maxlen
size_t strnlen(const char *s, size_t maxlen);

/// @brief compare two strings
/// @param s1 first string to compare
/// @param s2 second string to compare
/// @return 0 if s1 == s2, negative if s1 < s2, positive if s1 > s2
int strcmp(const char *s1, const char *s2);

/// @brief compare first n bytes of two strings
/// @param s1 first string to compare
/// @param s2 second string to compare
/// @param n first n bytes to compare
/// @return 0 if s1 == s2, negative if s1 < s2, positive if s1 > s2
int strncmp(const char *s1, const char *s2, size_t n);

/// @brief find character in string
/// @param s string to find the character c in
/// @param c character to find
/// @return return a pointer to the matched character or NULL if the character
/// is not found. The terminating null byte is considered part of the string,
/// so that if c is specified as '\0', these functions return a pointer to the
/// terminator.char *
char *strchr(const char *s, int c);

//// strings nonstandard //

/// @brief extract tokens from strings
/// @param str string to be broken into sequence of tokens
/// @param delim set of bytes that delimit tokens (may change between calls)
/// @return ptr to a null-terminated str containing the next token,
/// NULL if done
char *strtok(char *str, const char *delim);

/// @brief extract tokens from strings (re-entrant version)
/// @param str first call->string to be parsed (ignoring *saveptr),
/// subsequent calls->should be NULL
/// @param delim set of bytes that delimit tokens (may change between calls)
/// @param saveptr first call->ignored
/// @return ptr to a null-terminated str containing the next token,
/// NULL if done
char *strtok_r(char *str, const char *delim, char **saveptr);

/// @brief reverse a string
/// @param s string to reverse
void strrev(char *s);

/// @brief Convert integer to string, negative only if base 10
/// @param s destination, should be long enough to contain any value in the
/// selected base (32 in binary)
/// @param value integer to convert
/// @param base base to convert to, lowercase above 9, negative numbers only
/// supported for decimal
/// @return pointer to str on success, NULL with a non-valid base argument
char *itoa(char *str, int value, unsigned char base);

/// @brief Convert unsigned integer to string
/// @param s destination, should be long enough to contain any value in the
/// selected base (32 in binary)
/// @param value integer to convert
/// @param base base to convert to, lowercase above 9
/// @return pointer to str on success, NULL with a non-valid base argument
char *utoa(char *str, unsigned int value, unsigned char base);

/// x86 programming nonstandard ///

/// @brief returns a byte read from an I/O port
/// @param port address to read from
/// @return byte-sized content of the port
uint8_t inb(uint16_t port);

/// @brief write a byte to an I/O port
/// @param port address to write in
/// @param data value to write in port
void outb(uint16_t port, uint8_t data);

/// @brief Wait a very small amount of time (1 to 4 microseconds, generally).
/// Useful for implementing a small delay for PIC remapping on old hardware or
/// generally as a simple but imprecise wait.
void io_wait();

#endif
