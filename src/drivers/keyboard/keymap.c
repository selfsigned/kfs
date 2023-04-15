#include "keymap.h"

const kbd_entry layout_us_mode1[KBD_KEYS_END] = {
    [KBD_ESCAPE] = {0, KBD_ESCAPE},
    // KEY  FLAG  CODE NORMAL SHIFT ALT
    [KBD_1] = {0, KBD_1, '1', '!', '1'},
    [KBD_2] = {0, KBD_2, '2', '@', '2'},
    [KBD_3] = {0, KBD_3, '3', '#', '3'},
    [KBD_4] = {0, KBD_4, '4', '$', '4'},
    [KBD_5] = {0, KBD_5, '5', '%', '5'},
    [KBD_6] = {0, KBD_6, '6', '^', '6'},
    [KBD_7] = {0, KBD_7, '7', '&', '7'},
    [KBD_8] = {0, KBD_8, '8', '*', '8'},
    [KBD_9] = {0, KBD_9, '9', '(', '9'},
    [KBD_0] = {0, KBD_0, '0', ')', '0'},
    [KBD_DASH] = {0, KBD_DASH, '-', '_'},
    [KBD_EQUAL] = {0, KBD_EQUAL, '=', '+'},
    [KBD_BACKSPACE] = {0, KBD_BACKSPACE},
    [KBD_TAB] = {0, KBD_TAB},
    [KBD_Q] = {FLAG_CAPSLOCK, KBD_Q, 'q', 'Q'},
    [KBD_W] = {FLAG_CAPSLOCK, KBD_W, 'w', 'W'},
    [KBD_E] = {FLAG_CAPSLOCK, KBD_E, 'e', 'E'},
    [KBD_R] = {FLAG_CAPSLOCK, KBD_R, 'r', 'R'},
    [KBD_T] = {FLAG_CAPSLOCK, KBD_T, 't', 'T'},
    [KBD_Y] = {FLAG_CAPSLOCK, KBD_Y, 'y', 'Y'},
    [KBD_U] = {FLAG_CAPSLOCK, KBD_U, 'u', 'U'},
    [KBD_I] = {FLAG_CAPSLOCK, KBD_I, 'i', 'I'},
    [KBD_O] = {FLAG_CAPSLOCK, KBD_O, 'o', 'O'},
    [KBD_P] = {FLAG_CAPSLOCK, KBD_P, 'p', 'P'},
    [KBD_OPENING_BRACKET] = {0, KBD_OPENING_BRACKET, '[', '{'},
    [KBD_CLOSING_BRACKET] = {0, KBD_CLOSING_BRACKET, ']', '}'},
    [KBD_ENTER] = {0, KBD_ENTER},
    [KBD_LEFT_CTRL] = {0, KBD_LEFT_CTRL},
    [KBD_A] = {FLAG_CAPSLOCK, KBD_A, 'a', 'A'},
    [KBD_S] = {FLAG_CAPSLOCK, KBD_S, 's', 'S'},
    [KBD_D] = {FLAG_CAPSLOCK, KBD_D, 'd', 'D'},
    [KBD_F] = {FLAG_CAPSLOCK, KBD_F, 'f', 'F'},
    [KBD_G] = {FLAG_CAPSLOCK, KBD_G, 'g', 'G'},
    [KBD_H] = {FLAG_CAPSLOCK, KBD_H, 'h', 'H'},
    [KBD_J] = {FLAG_CAPSLOCK, KBD_J, 'j', 'J'},
    [KBD_K] = {FLAG_CAPSLOCK, KBD_K, 'k', 'K'},
    [KBD_L] = {FLAG_CAPSLOCK, KBD_L, 'l', 'L'},
    [KBD_COLON] = {0, KBD_COLON, ';', ':'},
    [KBD_SINGLE_QUOTE] = {0, KBD_SINGLE_QUOTE, '\'', '\"'},
    [KBD_BACKTICK] = {0, KBD_BACKTICK, '`', '~'},
    [KBD_LEFT_SHIFT] = {0, KBD_LEFT_SHIFT},
    [KBD_BACKSLASH] = {0, KBD_BACKSLASH, '\\', '|'},
    [KBD_Z] = {FLAG_CAPSLOCK, KBD_Z, 'z', 'Z'},
    [KBD_X] = {FLAG_CAPSLOCK, KBD_X, 'x', 'X'},
    [KBD_C] = {FLAG_CAPSLOCK, KBD_C, 'c', 'C'},
    [KBD_V] = {FLAG_CAPSLOCK, KBD_V, 'v', 'V'},
    [KBD_B] = {FLAG_CAPSLOCK, KBD_B, 'b', 'B'},
    [KBD_N] = {FLAG_CAPSLOCK, KBD_N, 'n', 'N'},
    [KBD_M] = {FLAG_CAPSLOCK, KBD_M, 'm', 'M'},
    [KBD_COMMA] = {0, KBD_COMMA, ',', '<'},
    [KBD_DOT] = {0, KBD_DOT, '.', '>'},
    [KBD_SLASH] = {0, KBD_SLASH, '/', '?'},
    [KBD_RIGHT_SHIFT] = {0, KBD_RIGHT_SHIFT},
    [KBD_KEYPAD_ASTERISK] = {0, KBD_KEYPAD_ASTERISK, '*', '*'},
    [KBD_LEFT_ALT] = {0, KBD_LEFT_ALT},
    [KBD_SPACE] = {0, KBD_SPACE, ' ', ' '},
    [KBD_CAPSLOCK] = {FLAG_TOGGLE, KBD_CAPSLOCK, ' ', ' '},
    [KBD_F1] = {0, KBD_F3},
    [KBD_F2] = {0, KBD_F2},
    [KBD_F3] = {0, KBD_F3},
    [KBD_F4] = {0, KBD_F4},
    [KBD_F5] = {0, KBD_F5},
    [KBD_F6] = {0, KBD_F6},
    [KBD_F7] = {0, KBD_F7},
    [KBD_F8] = {0, KBD_F8},
    [KBD_F9] = {0, KBD_F9},
    [KBD_NUMLOCK] = {FLAG_TOGGLE, KBD_CAPSLOCK, ' ', ' '},
    [KBD_F10] = {0, KBD_F10},
    [KBD_F11] = {0, KBD_F11},
    [KBD_F12] = {0, KBD_F12},

    // extended keys (0xE0)
    [KBD_KEYPAD_ENTER] = {0, KBD_KEYPAD_ENTER},
    [KBD_RIGHT_CTRL] = {0, KBD_RIGHT_CTRL},
    [KBD_KEYPAD_SLASH] = {0, KBD_KEYPAD_SLASH, '/', '/'},
    [KBD_RIGHT_ALT] = {0, KBD_RIGHT_ALT},
    [KBD_HOME] = {0, KBD_HOME},
    [KBD_CURSOR_UP] = {0, KBD_CURSOR_UP},
    [KBD_PAGE_UP] = {0, KBD_PAGE_UP},
    [KBD_CURSOR_LEFT] = {0, KBD_CURSOR_LEFT},
    [KBD_CURSOR_RIGHT] = {0, KBD_CURSOR_RIGHT},
    [KBD_END] = {0, KBD_END},
    [KBD_CURSOR_DOWN] = {0, KBD_CURSOR_DOWN},
    [KBD_PAGE_DOWN] = {0, KBD_PAGE_DOWN},
    [KBD_INSERT] = {0, KBD_INSERT},
    [KBD_DELETE] = {0, KBD_DELETE},
};
