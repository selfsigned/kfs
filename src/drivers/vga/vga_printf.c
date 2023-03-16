#include "vga.h"
#include "vga_internal.h"

/// @brief implement printf argument reading
/// @return numbers of character written, negative if error
int vga_printf_readarg(vga_info *info, const char *format, va_list *ap,
                       size_t *i) {
  int result = 0;
  unsigned char tmp[64] = {0};

  // target: %[flags][width][.precision][length]type
  switch (format[*i]) {
  case '%':
    result += vga_buffer_writechar(info, '%');
    break;
  case 'c':
    result += vga_buffer_writechar(info, (unsigned char)va_arg(*ap, int));
    break;
  case 'C':
    // not unicode but CP437 instead, extension
    result +=
        vga_buffer_writechar(info, (unsigned char)va_arg(*ap, unsigned int));
    break;
  case 's':
    // ascii string, includes '\n'
    result +=
        vga_buffer_write(info, (unsigned char *)va_arg(*ap, char *), true);
    break;
  case 'S':
    // CP437
    result += vga_buffer_write(
        info, (unsigned char *)va_arg(*ap, unsigned char *), false);
    break;
  case 'u':
    utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 10);
    vga_buffer_write(info, tmp, false);
    // base 10: unsigned
    break;
  case 'i':
    itoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 10);
    vga_buffer_write(info, tmp, false);
    break;
  case 'x':
    utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 16);
    vga_buffer_write(info, tmp, false);
    // base 10: integer
    break;
  case 'o':
    utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 8);
    vga_buffer_write(info, tmp, false);
    // base 8: octal
    break;
  case 'b':
    // base 2: binary, extension
    utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 2);
    vga_buffer_write(info, tmp, false);
    break;
  case 'a':
    // vga attributes, extension
    vga_screen_setattributes(info->screen,
                             (vga_attributes)va_arg(*ap, vga_attributes));
    break;
  default:
    // Not implemented yet
    return -1;
  }
  return result;
}

int vga_vdprintf(vga_info info, const char *format, va_list ap) {
  int result = 0;
  int ret = 0;

  // Parse the format string
  for (size_t i = 0; format[i]; ++i) {
    switch (format[i]) {
    case '%':
      ++i;
      if ((ret = vga_printf_readarg(&info, format, &ap, &i) < 0))
        return -1;
      result += ret;
      break;
    case '\n':
      vga_set_cursor(&info, true);
      break;
    default:
      result += vga_buffer_writechar(&info, format[i]);
    }
  }

  if (info.print)
    vga_screen_show(info.screen);

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
