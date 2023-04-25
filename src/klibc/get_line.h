#ifndef GET_LINE_H
#define GET_LINE_H
// Separate get_line from the klibc because of the reliance on the vga and
// keyboard drivers

#include <stddef.h>
#include <stdint.h>

/// @brief Reads a line of input (ends at '\n')
/// @param dest destination buffer, needs to be at least maxlen big
/// @param screen -1 not to output
/// @param maxlen max number of characters written to input
/// @return number of characters read, 0 if C^c pressed
size_t get_line(char *dest, int screen, size_t maxlen);

#endif
