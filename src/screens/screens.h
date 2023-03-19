#ifndef SCREENS_H
#define SCREENS_h

#include <stdint.h>

/// @brief everyone's favorite, lorem ipsum
/// @param screen_nbr screen to ipsum to
void screen_lorem_ipsum(uint8_t screen_nbr);

/// @brief loop handling screen selection
/// @param home_screen nbr of the home_screen
/// @param note_screen nbr of the note screen
void screen_loop(uint8_t home_screen, uint8_t note_screen);

#endif
