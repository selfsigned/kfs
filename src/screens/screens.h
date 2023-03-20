#ifndef SCREENS_H
#define SCREENS_h

#include <stdint.h>

// Screens

/// @brief everyone's favorite, lorem ipsum
/// @param screen_nbr screen to ipsum to
void screen_lorem_ipsum(uint8_t screen_nbr);

/// @brief loop handling screen selection
/// @param home_screen nbr of the home_screen
/// @param note_screen nbr of the note screen
void screen_loop(uint8_t home_screen, uint8_t note_screen);

// Display helpers

/// @brief create a light box on a given screen starting at the cusor
/// @param screen_nbr nbr of the screen to write to
/// @param width width of the box, starting from 0
/// @param height height of the box, starting from 0
/// @return false if box too big or the screen doesn't exist
bool screen_create_box_light(uint8_t screen_nbr, uint8_t width, uint8_t height);

#endif
