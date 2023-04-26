#include "cmd_hexdump.h"
#include "../../cp437.h"
#include "../../klibc/libc.h"

void hexdump(uint8_t screen_nbr, void *data, size_t size) {
  vga_info screen_info = {.screen = screen_nbr};
  const uint8_t *p = (const uint8_t *)data;
  for (size_t i = 0; i < size; i += 16) {
    vga_printf(screen_info, "%10p: ", (uint32_t)data + i);
    for (size_t j = 0; j < 16; j++) {
      if (i + j < size) {
        vga_printf(screen_info, "%02X ", p[i + j]);
      } else {
        vga_printf(screen_info, "   ");
      }
    }
    vga_printf(screen_info, "%C", CP437_BOX_DRAWINGS_LIGHT_VERTICAL);
    for (size_t j = 0; j < 16; j++) {
      if (i + j < size) {
        uint8_t c = p[i + j];
        if (c >= 32 && c <= 126) {
          vga_printf(screen_info, "%c", c);
        } else {
          vga_printf(screen_info, ".");
        }
      } else {
        vga_printf(screen_info, " ");
      }
    }
    vga_printf(screen_info, "%C\n", CP437_BOX_DRAWINGS_LIGHT_VERTICAL);
  }
}

int cmd_hexdump(uint8_t screen_nbr, int ac, char **av) {
  vga_info screen_info = {.screen = screen_nbr};
  int input_size;
  void *data;

  if (ac < 1) {
    vga_printf(screen_info, CMD_HEXDUMP_USAGE);
    return 1;
  }

  // address
  data = (void *)atou_base(av[0], 16);

  // size
  if (ac > 1) {
    input_size = atoi(av[1]);
  } else
    input_size = CMD_HEXDUMP_DEFAULT_SIZE;
  if (input_size < 0) {
    vga_printf(screen_info, "Error: size can't be negative");
    return 1;
  }

  hexdump(screen_nbr, data, (size_t)input_size);
  return 0;
}
