#include "vga.h"
#include "vga_internal.h"

// http://www.osdever.net/FreeVGA/vga/crtcreg.htm

void vga_crtc_disable_cursor() {
  // Select the cursor start register in the CRTC
  outb(VGA_CRTC_INDEX, VGA_CRTC_REG_CURSOR_START);
  // Write the CD bit
  outb(VGA_CRTC_DATA, 1 << 5);
}

void vga_crtc_enable_cursor(uint8_t scanline_start, uint8_t scanline_end) {
  // protect from overflow
  if (scanline_start > 15 || scanline_end > 15) {
    scanline_start = 0;
    scanline_end = 15;
  }

  outb(VGA_CRTC_INDEX, VGA_CRTC_REG_CURSOR_START); // select the start register
  outb(VGA_CRTC_DATA, scanline_start); // disable CD and set cusor start

  outb(VGA_CRTC_INDEX, VGA_CRTC_REG_CURSOR_END);
  outb(VGA_CRTC_DATA, scanline_end);
}

void vga_crtc_set_cursor(uint8_t column, uint8_t row) {
  uint16_t position = row * VGA_COL + column;

  // HIGH cursor byte
  outb(VGA_CRTC_INDEX, VGA_CRTC_REG_CURSOR_LOCATION_HIGH);
  outb(VGA_CRTC_DATA, position >> 8);

  // LOW cursor byte
  outb(VGA_CRTC_INDEX, VGA_CRTC_REG_CURSOR_LOCATION_LOW);
  outb(VGA_CRTC_DATA, position);
}
