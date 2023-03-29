#include "printf_internal.h"
#include "vga.h"
#include "vga_internal.h"
#include <stdarg.h>

//  Flags and string impl:
// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap05.html

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
      while (isdigit(*format))
        format++;
    }

    // precision
    if (*format == '.') {
      params.flags.precision = true;
      format++;

      params.precision = atoi(format);
      while (isdigit(*format))
        format++;
    }

    // type
    switch (*format) {

    // vga attributes, extension
    case 'a': {
      vga_screen_setattributes(info.screen,
                               (vga_attributes)va_arg(ap, vga_attributes));
      format++;
      break;
    }

    // strings
    case 'S':
      info.internal.cp437_print = true; // fall through
    case 's': {
      unsigned char *str = (unsigned char *)va_arg(ap, unsigned char *);
      if (!*str) {
        str = (unsigned char *)"(null)";
      }

      // max value if no precision for strnlen
      size_t len =
          strnlen(str, params.flags.precision ? params.precision : (size_t)-1);

      // left justify
      if (!params.flags.left_justify) {
        while (len++ < params.width)
          print_func(&info, &result, (unsigned char *)" ");
      }

      // print str
      if (params.flags.precision && params.precision <= len) {
        // prematurely end str and then restore it
        tmp[0] = str[params.precision];
        str[params.precision] = '\0';
        print_func(&info, &result, str);
        str[params.precision] = tmp[0];
      } else
        print_func(&info, &result, str);

      // right justify
      if (params.flags.left_justify) {
        while (len++ < params.width)
          print_func(&info, &result, (unsigned char *)" ");
      }

      format++;
      break;
    }

    // char
    case 'C':
      info.internal.cp437_print = true; // fall through
    case 'c': {
      tmp[0] = (unsigned char)va_arg(ap, int);
      size_t len = 1;

      // left justify
      if (!params.flags.left_justify) {
        while (len++ < params.width)
          print_func(&info, &result, (unsigned char *)" ");
      }

      print_func(&info, &result, tmp);

      // right justify
      if (params.flags.left_justify) {
        while (len++ < params.width)
          print_func(&info, &result, (unsigned char *)" ");
      }

      format++;
      break;
    }

    // numbers
    case 'd': // fall through
    case 'i': // fall through
    case 'o': // fall through
    case 'u': // fall through
    case 'x': // fall through
    case 'X': {
      if (*format == 'X' || *format == 'x') {
        params.base = 16;
      } else if (*format == 'o') {
        params.base = 8;
      } else if (*format == 'b') {
        params.base = 2;
      } else {
        params.base = 10;
      }

      // tmp[0] = (unsigned char)va_arg(ap, int);
      // print_func(&info, &result, utoa(tmp));
      format++;
      break;
    }

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
