#include "pic.h"
#include "../klibc/libc.h"

void init_pic(uint8_t master_vector_offset, uint8_t slave_vector_offset) {
  // Initialization happens in 3 to 4 Init Control Words
  icw1_t icw1 = {.data = {
                     .enable = true,
                     .icw4_needed = true, // needed to set 8086 mode
                 }};
  const uint8_t slave_irq = 2;
  icw4_t icw4 = {.data = {
                     .mode_8086 = true,
                 }};

  // ICW1 (sent to the control register)
  outb(PIC1, icw1.byte);
  io_wait();
  outb(PIC2, icw1.byte);
  io_wait();

  // ICW2 (vector offset)
  outb(PIC1_DATA, master_vector_offset);
  io_wait();
  outb(PIC2_DATA, slave_vector_offset);
  io_wait();

  // ICW3 (master/slave addressing)
  outb(PIC1_DATA, 1 << slave_irq); // IR input, master has slave PIC at IRQ2
  io_wait();
  outb(PIC2_DATA, slave_irq); // Slave ID is equal to master IR input
  io_wait();

  // ICW4
  outb(PIC1_DATA, icw4.byte);
  io_wait();
  outb(PIC2_DATA, icw4.byte);
  io_wait();

  // Mask interrupts
  outb(PIC1_DATA, 0xff);
  io_wait();
  outb(PIC2_DATA, 0xff);
  io_wait();
}
