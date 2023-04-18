#include "pic.h"
#include "../../klibc/libc.h"
#include "interrupts.h"

void pic_init(uint8_t master_vector_offset, uint8_t slave_vector_offset) {
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
  INFO_MSG("PIC initialized (master vector offset:%d slave vector offset:%d)",
           master_vector_offset, slave_vector_offset);
}

void pic_send_eoi(uint8_t irq) {
  if (irq >= 8)
    outb(PIC2, PIC_EOI);
  outb(PIC1, PIC_EOI);
}

void pic_mask(uint8_t irq) {
  uint16_t port = PIC1_DATA;

  if (irq >= 8) {
    irq -= 8;
    port = PIC2_DATA;
  }
  outb(port, inb(port) | (1 << irq));
}

void pic_unmask(uint8_t irq) {
  uint16_t port = PIC1_DATA;

  if (irq >= 8) {
    irq -= 8;
    port = PIC2_DATA;
  }
  outb(port, inb(port) & ~(1 << irq));
}

uint16_t pic_get_irq_reg(ocw3_t ocw3) {
  outb(PIC1, ocw3);
  outb(PIC2, ocw3);
  return (inb(PIC2) << 8) | inb(PIC1);
}
