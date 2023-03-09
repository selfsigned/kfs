#include "vga.h"
#include "vga_internal.h"

/// @brief
/// @return
int vga_printf_readarg(vga_info *info, const char *format, va_list *ap,
                       size_t *i) {
  int result = 0;

  // target: %[flags][width][.precision][length]type
  switch (format[*i]) {
  case '%':
    result += vga_buffer_writechar(info, '%');
    break;
  case 'c':
    result += vga_buffer_writechar(info, (unsigned char)va_arg(*ap, int));
    break;
  case 'C':
    // not unicode but CP437 instead
    result +=
        vga_buffer_writechar(info, (unsigned char)va_arg(*ap, unsigned int));
    break;
  case 's':
    result += vga_buffer_write(info, (unsigned char *)va_arg(*ap, char *));
    break;
  case 'S':
    // CP437
    result +=
        vga_buffer_write(info, (unsigned char *)va_arg(*ap, unsigned char *));
    break;
  default:
    // Not implemented yet
    break;
  }
  return result;
}

int vga_vdprintf(vga_info info, const char *format, va_list ap) {
  int result = 0;

  // Parse the format string
  for (size_t i = 0; format[i]; ++i) {
    switch (format[i]) {
    case '%':
      ++i;
      result += vga_printf_readarg(&info, format, &ap, &i);
      break;
    case '\n':
      info.column = 0;
      info.row++;
      break;
    default:
      result += vga_buffer_writechar(&info, format[i]);
    }
  }

  if (info.print)
    vga_showscreen(info.screen);

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
