#ifndef GDT_H
#define GDT_H

#include <stdint.h>
struct segment_desc
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t limit_high:4;
  uint8_t granularity:4;
  uint8_t base_high;

} __attribute__ ((packed));

struct gdt_ptr
{
  uint16_t limit; //size of gdt
  uint32_t base;  // address
} __attribute__((packed));


void init_gdt();

#endif
