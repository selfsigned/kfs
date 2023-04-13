#include "keyboard.h"
#include "../../hardware/interrupts/interrupts.h"
#include "../../klibc/libc.h"
#include "ps2.h"

struct kbd_state kbd = {};
uint8_t kbd_led_mask = 0;

// _ _|      |                     |
//   |    \   _|  -_)  _| \   _` | |
// ___|_| _|\__|\___|_|_| _|\__,_|_|

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

static void _kbd_set_led(enum KBD_ED indicator, bool enabled) {
  kbd_led_mask =
      (enabled) ? kbd_led_mask | indicator : kbd_led_mask ^ indicator;

  // wait for the input register to be ready
  while ((inb(KBD_IO_STATUS_REGISTER) & 2)) {
  }
  // select the indicator register
  outb(KBD_IO_DATA_PORT, 0xED);
  // wait for the input register to be ready again
  while ((inb(KBD_IO_STATUS_REGISTER) & 2)) {
  }
  // finally send our desired indicator state
  outb(KBD_IO_DATA_PORT, kbd_led_mask);
}

static void _kbd_set_modifiers(scancode key, bool lock_modifiers, bool enable) {
  // TODO doesn't work (duh)
  if (lock_modifiers) {
    switch (key) {

    case KBD_CAPSLOCK:
      kbd.mod.capslock ^= enable;
      kbd_led_caps(enable);
      break;
    case KBD_NUMLOCK:
      kbd.mod.numlock ^= enable;
      kbd_led_num(enable);
      break;
    case KBD_SCROLLLOCK:
      kbd.mod.scroll_lock ^= enable;
      kbd_led_scroll(enable);
      break;

    default:
      break;
    }
  }

  switch (key) {

  case KBD_RIGHT_SHIFT: // fall through
  case KBD_LEFT_SHIFT:
    kbd.mod.shift = enable;
    break;

  case KBD_LEFT_CTRL:
    // case KBD_RIGHT_CTRL: // TODO charset 2
    kbd.mod.ctrl = enable;
    break;

  case KBD_LEFT_ALT:
    // case KBD_RIGHT_ALT: // TODO charset 2
    kbd.mod.alt = enable;
    break;

  default:
    break;
  }
}

INTERRUPT static void kbd_handler(int_frame *frame) {
  INT_SPURIOUSIRQ_GUARD(IRQ_PS2_KEYBOARD)

  if (inb(KBD_IO_STATUS_REGISTER) & 1) { // kbd should be ready
    kbd.key = inb(KBD_IO_DATA_PORT);     // empty the data buffer
    kbd.ascii = kbd_code_to_ascii(kbd.key);

    // TODO enable scan code Set 2 support

    // enable modifiers
    _kbd_set_modifiers(kbd.key, false, true);
    // disable modifiers
    if (kbd.key & KBD_RELEASED_OFFSET)
      _kbd_set_modifiers(kbd.key ^ KBD_RELEASED_OFFSET, true, false);

    if (kbd.mod.shift || kbd.mod.capslock)
      // TODO switch to different mod
      kbd.ascii = toupper(kbd.ascii);
  }

  int_irq_end(IRQ_PS2_KEYBOARD);
}

//    \   _ \_ _|
//   _ \  __/  |
// _/  _\_|  ___|

void kbd_init() { int_irq_add(IRQ_PS2_KEYBOARD, &kbd_handler); }

scancode kbd_poll() {
  return (inb(KBD_IO_STATUS_REGISTER) & 1) ? inb(KBD_IO_DATA_PORT) : 0;
}

char kbd_code_to_ascii(scancode code) {
  return (code <= 127) ? us_scancode_1[code] : 0;
}

void kbd_led_scroll(bool enabled) {
  _kbd_set_led(KBD_STATUS_INDICATOR_SCROLL, enabled);
}
void kbd_led_num(bool enabled) {
  _kbd_set_led(KBD_STATUS_INDICATOR_NUM, enabled);
}
void kbd_led_caps(bool enabled) {
  _kbd_set_led(KBD_STATUS_INDICATOR_CAPS, enabled);
}
