#include "printf_internal.h"
#include "vga.h"
#include "vga_internal.h"
#include <stdarg.h>

//  Flags and string impl:
// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap05.html

// TODO provide mechanism to abstract printf out of vga driver, pointer on
// function for screen printing could work

//   // number types
//   case 'i':
//     // base 10: integer
//     itoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 10);
//     vga_buffer_write(info, tmp, false);
//     break;
//   case 'u':
//     // base 10: unsigned
//     utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 10);
//     vga_buffer_write(info, tmp, false);
//     break;
//   case 'x':
//     // base 16: unsigned hexa
//     utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 16);
//     vga_buffer_write(info, tmp, false);
//     break;
//   case 'o':
//     // base 8: octal
//     utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 8);
//     vga_buffer_write(info, tmp, false);
//     break;
//   // custom types
//   case 'b':
//     // base 2: binary, extension
//     utoa((char *)tmp, (unsigned int)va_arg(*ap, unsigned int), 2);
//     vga_buffer_write(info, tmp, false);
//     break;

int vga_vdprintf(vga_info info,
                 void (*print_func)(vga_info *, size_t *, unsigned char *),
                 const char *format, va_list ap) {
  int ret = 0;
  size_t result = 0;

  // fmt string: %[flags][width][.precision]type
  // missing: [length]
  while (*format) {
    unsigned char tmp[64] = {0};
    struct printf_parameters params = {};
    info.internal.cp437_print = false; // reset str state

    // print non-specifiers
    if (*format != '%') {
      tmp[0] = *format;
      print_func(&info, &result, tmp);
      format++;
      continue;
    }
    format++;

    // flags
    switch (*format) {
    // left-justify
    case '-':
      params.flags.left_justify = true;
      format++;
      break;
    // always add a sign
    case '+':
      params.flags.add_plus = true;
      format++;
      break;
    // begin with space if no sign
    case ' ':
      params.flags.add_space = true;
      format++;
      break;
    // 0 pad [width]
    case '0':
      params.flags.zero_pad = true;
      format++;
      break;
    // enable alternative form
    case '#':
      params.flags.alt_form = true;
      format++;
      break;
    // INOP integer exponent thousand separator, I so can't be bothered
    case '\'':
    default:
      break;
    }

    // width
    if (isalnum(*format)) {
      params.width = atoi(format);
    }

    // precision
    if (*format == '.') {
      params.flags.precision = true;
      format++;

      params.precision = atoi(format);
    }

    // type
    switch (*format) {
    case 'a':
      // vga attributes, extension
      vga_screen_setattributes(info.screen,
                               (vga_attributes)va_arg(ap, vga_attributes));
      format++;
      break;

    // TODO precision width

    // strings
    case 'S':
      info.internal.cp437_print = true; // fall through
    case 's':
      print_func(&info, &result, (unsigned char *)va_arg(ap, unsigned char *));
      format++;
      break;

    // char
    case 'C':
      info.internal.cp437_print = true; // fall through
    case 'c':
      tmp[0] = (unsigned char)va_arg(ap, int);
      print_func(&info, &result, tmp);
      format++;
      break;

    case '%': // fall through
    default:
      tmp[0] = *format;
      print_func(&info, &result, tmp);
      format++;
    }
  }

  return result;
}

int vga_printf(vga_info info, const char *format, ...) {
  int result;
  va_list ap;

  va_start(ap, format);
  result = vga_vdprintf(info, vga_buffer_write, format, ap);
  va_end(ap);

  if (info.print)
    vga_screen_show(info.screen);

  return result;
}
