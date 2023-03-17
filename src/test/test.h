#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// @brief executes a range of visual tests
/// @param screen_nbr screen to write to
void test_printf(uint8_t screen_nbr);

/// @brief display the whole cp437 charset
void test_vga_cp437(uint8_t screen_nbr);
#endif
