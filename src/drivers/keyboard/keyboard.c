#include "keyboard.h"
#include "../../hardware/interrupts/interrupts.h"
#include "../../klibc/libc.h"
#include "../../log.h"
#include "layout.h"
#include "ps2.h"

struct kbd_state kbd = {};
uint8_t kbd_led_mask = 0;

// _ _|      |                     |
//   |    \   _|  -_)  _| \   _` | |
// ___|_| _|\__|\___|_|_| _|\__,_|_|

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

static void _kbd_set_modifiers(kbd_scancode key, enum kbd_event status) {
  bool enable = (status == KEY_PRESSED);

  switch (key) {

  case KBD_CAPSLOCK: // I guess we actually don't need the TOGGLE flag
    if (kbd.status == KEY_RELEASED) {
      kbd.mod.capslock ^= true;
      kbd_led_caps(kbd.mod.capslock);
    }
    break;

  case KBD_NUMLOCK:
    if (kbd.status == KEY_RELEASED) {
      kbd.mod.numlock ^= true;
      kbd_led_num(kbd.mod.numlock);
    }
    break;

  case KBD_SCROLLLOCK:
    if (kbd.status == KEY_RELEASED) {
      kbd.mod.scroll_lock ^= true;
      kbd_led_scroll(kbd.mod.scroll_lock);
    }
    break;

  case KBD_RIGHT_SHIFT: // fall through
  case KBD_LEFT_SHIFT:
    kbd.mod.shift = enable;
    break;

  case KBD_LEFT_CTRL: // fall through
  case KBD_RIGHT_CTRL:
    kbd.mod.ctrl = enable;
    break;

  case KBD_LEFT_ALT: // fall through
  case KBD_RIGHT_ALT:
    kbd.mod.alt = enable;
    break;

  default:
    break;
  }
}

INTERRUPT static void kbd_handler(int_frame *frame) {
  INT_SPURIOUSIRQ_GUARD(IRQ_PS2_KEYBOARD);
  (void)frame; // needed for attributes but we don't care

  if (inb(KBD_IO_STATUS_REGISTER) & 1) { // kbd should be ready
    static bool extended_keys = false;
    kbd_entry entry;

    kbd_scancode code = inb(KBD_IO_DATA_PORT); // receive keycode
    if (extended_keys)                         // detect if extended key
      code += KBD_EXTENDED_KEYS;               // apply offset

    // detect key position and apply offset correction
    if ((code < KBD_EXTENDED_KEYS && code > KBD_RELEASED_OFFSET) ||
        (extended_keys && code > KBD_EXTENDED_KEYS_END)) {
      kbd.status = KEY_RELEASED;
      code -= KBD_RELEASED_OFFSET;
    } else
      kbd.status = KEY_PRESSED;

    entry = kbd_layout[code]; // get key in layout
    kbd.key = entry.code;     // map keycode depending on layout

    // set modifiers
    if (entry.code)
      _kbd_set_modifiers(kbd.key, kbd.status);

    // get ascii value depending on modifiers
    if (kbd.mod.shift || (kbd.mod.capslock && (entry.flags & FLAG_CAPSLOCK)))
      kbd.ascii = entry.shift;
    else if (kbd.mod.alt)
      kbd.ascii = entry.alt;
    else
      kbd.ascii = entry.normal;

    // set extended keys state (E0h)
    extended_keys = (code == KBD_EXTENDED_KEYS);
    DEBUG_KBD_MSG("event:%i,received:%#.3x,mapped:%#.3x,ascii: %c", kbd.status,
                  code, kbd.key, kbd.ascii);
  } else {
    WARN_MSG("Keyboard interrupt fired before 8042 controller was ready");
  }

  int_irq_end(IRQ_PS2_KEYBOARD); // signal end of interrupt to the PIC
}

//    \   _ \_ _|
//   _ \  __/  |
// _/  _\_|  ___|

void kbd_init() {
  INFO_MSG("Initializing Keyboard...");

  // disable leds //
  _kbd_set_led(KBD_STATUS_INDICATOR_CAPS, false);
  _kbd_set_led(KBD_STATUS_INDICATOR_NUM, false);
  _kbd_set_led(KBD_STATUS_INDICATOR_SCROLL, false);

  // Select scan code set 1 //

  // wait for the input register to be ready
  while ((inb(KBD_IO_STATUS_REGISTER) & 2))
    ;
  outb(KBD_IO_DATA_PORT, KBD_IO_SCANCODE_REGISTER);
  // wait for the input register to be ready again
  while ((inb(KBD_IO_STATUS_REGISTER) & 2))
    ;
  outb(KBD_IO_DATA_PORT, KBD_SCANCODE_SET1);

  // Setup the keyboard handler interrupt //
  int_irq_add(IRQ_PS2_KEYBOARD, &kbd_handler);

  INFO_MSG("Keyboard handler(%p) initialized", kbd_handler);
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

kbd_scancode kbd_poll() {
  while (!(inb(KBD_IO_STATUS_REGISTER) & 1))
    ;
  return inb(KBD_IO_DATA_PORT);
}
