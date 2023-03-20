#include "keyboard.h"
#include "../../klibc/libc.h"
#include "keyboard_internal.h"

// TODO use partial init with ranges
// http://www.osdever.net/bkerndev/Docs/keyboard.htm
unsigned char us_scancode_1[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '*',  0,    ' ', 0,    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   '+', 0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0,
};

scancode kbd_get() {
  return (inb(KBD_IO_STATUS_REGISTER) & 1) ? inb(KBD_IO_DATA_PORT) : 0;
}

char kbd_code_to_ascii(scancode code) {
  return (code <= 127) ? us_scancode_1[code] : 0;
}
