#include "vga.h"
#include "vga_internal.h"

int vga_vdprintf(vga_info info, const char *format, va_list ap) {
  int result = 0;

  // Parse the format string
  for (size_t i = 0; format[i]; ++i) {
    if (format[i] == '%') {
      // TODO
    } else {
      result += vga_buffer_write(&info, (vga_char){format[i]});
    }
  }

  if (!info.noprint)
    vga_buffer_flush(info);

  return result;
}

int vga_printf(vga_info info, const char *format, ...) {
  int result;
  va_list ap;

  va_start(ap, format);
  result = vga_vdprintf(info, format, ap);
  va_end(ap);
  return result;
}
