#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_NB_ENTRIES 7
#define GDT_ADDRESS 0x800

struct gdt_segment_desc {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t limit_high : 4;
  uint8_t granularity : 4;
  uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
  uint16_t limit; // size of gdt
  uint32_t base;  // address
} __attribute__((packed));

/// @brief  Our gdt
extern struct gdt_segment_desc *gdt;
/// @brief  pointer to our gdt
extern struct gdt_ptr gdt_ptr;

/// @brief init gdt_pointer in flat mode and fill the gdt
void gdt_init();

#endif
