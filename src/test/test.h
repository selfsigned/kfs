#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// @brief executes a range of visual tests
/// @param screen_nbr screen to write to
/// @param print print the test or not
/// @param scroll print scrolled or not
void test_vga(uint8_t screen_nbr, bool scroll, bool print);

#endif
