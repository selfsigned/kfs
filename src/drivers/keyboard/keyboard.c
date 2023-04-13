#include "keyboard.h"
#include "../../hardware/interrupts/interrupts.h"
#include "../../klibc/libc.h"
#include "ps2.h"

struct kbd_state kbd = {};

// TODO use partial init with ranges
// http://www.osdever.net/bkerndev/Docs/keyboard.htm
unsigned char us_scancode_1[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '*',  0,    0,   0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   '+', 0,
};

INTERRUPT static void kbd_handler(int_frame *frame) {
  INT_SPURIOUSIRQ_GUARD(IRQ_PS2_KEYBOARD)

  // read and empty the buffer
  if (inb(KBD_IO_STATUS_REGISTER) & 1) {
    kbd.key = inb(KBD_IO_DATA_PORT);
    kbd.ascii = kbd_code_to_ascii(kbd.key);

    switch (kbd.key) {
    case KBD_RIGHT_SHIFT: // fall through
    case KBD_LEFT_SHIFT: {
      kbd.mod.shift = true;
    }
    default:
      break;
    }
  }

  int_irq_end(IRQ_PS2_KEYBOARD);
}

scancode kbd_poll() {
  return (inb(KBD_IO_STATUS_REGISTER) & 1) ? inb(KBD_IO_DATA_PORT) : 0;
}

char kbd_code_to_ascii(scancode code) {
  return (code <= 127) ? us_scancode_1[code] : 0;
}

void kbd_init() { int_irq_add(IRQ_PS2_KEYBOARD, &kbd_handler); }
