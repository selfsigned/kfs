#ifndef PS2_KBD_H
#define PS2_KBD_H

// 8042 controller interfaces

// PS/2 controller I/O registers
#define KBD_IO_DATA_PORT 0x60
#define KBD_IO_STATUS_REGISTER 0x64
#define KBD_IO_COMMAND_REGISTER 0x64
#define KBD_IO_INDICATOR_REGISTER 0xED

/// |7-3|2|1|0| Keyboard Status Indicator Option Byte
///   |  | | `--- Scroll-Lock indicator  (0=off, 1=on)
///   |  | `---- Num-Lock indicator  (0=off, 1=on)
///   |  `----- Caps-Lock indicator  (0=off, 1=on)
///   `------- reserved (must be zero)
enum KBD_ED {
  KBD_STATUS_INDICATOR_SCROLL = 0x1,
  KBD_STATUS_INDICATOR_NUM = 0x2,
  KBD_STATUS_INDICATOR_CAPS = 0x4,
};

// command to shutdown the cpu from the KBD controller
#define KBD_IO_SHUTDOWN 0xFE

#endif
